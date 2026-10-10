#!/usr/bin/env python3
"""Send/receive FOSD wavelength packets using the local KM-TEST adapter.

Defaults: sender 192.168.31.89 -> receiver 192.168.31.88:5060, UDP.
Only Python's standard library is required. Run --help for examples/options.
"""

import argparse
import ipaddress
import math
import random
import socket
import struct
import sys
import time

HEADER = struct.Struct("<HBBHI")
RECORD = struct.Struct("<BBf")
DEFAULT_SOURCE = "192.168.31.89"
DEFAULT_TARGET = "192.168.31.88"


def validate_records(records):
    if len(records) > 1024:
        raise ValueError("At most 1024 records are supported")
    identities = set()
    for channel, sensor, wave in records:
        if not 1 <= channel <= 16 or not 1 <= sensor <= 255:
            raise ValueError("channel must be 1..16, sensor must be 1..255")
        if (channel, sensor) in identities:
            raise ValueError("Duplicate channel/sensor")
        identities.add((channel, sensor))
        if not math.isfinite(wave) or abs(wave) > 3.4028234663852886e38:
            raise ValueError("Wavelength must be a finite float32")


def build_packet(records, protocol="new", sequence=0, version=1, data_type=2):
    validate_records(records)
    if protocol == "new":
        return HEADER.pack(0xA55A, version, data_type, len(records), sequence & 0xFFFFFFFF) + b"".join(
            RECORD.pack(*record) for record in records
        )
    if protocol != "legacy":
        raise ValueError("Unknown packet protocol")
    if len(records) > 255:
        raise ValueError("Legacy packets support at most 255 records")
    records = sorted(records)
    counts = [0] * 16
    for channel, sensor, _ in records:
        counts[channel - 1] += 1
        if sensor != counts[channel - 1]:
            raise ValueError("Legacy sensor IDs must be consecutive starting at 1 in each channel")
    header = bytearray(24)
    header[:4] = bytes.fromhex("fa fb fa fb")
    header[4:20] = bytes(counts)
    header[20] = len(records)
    return bytes(header) + b"".join(struct.pack("<f", wave) for _, _, wave in records)


def decode_packet(packet):
    if packet[:2] == b"\x5a\xa5":
        if len(packet) < HEADER.size:
            raise ValueError("Truncated new header")
        _, version, data_type, count, sequence = HEADER.unpack_from(packet)
        if count > 1024 or len(packet) != HEADER.size + count * RECORD.size:
            raise ValueError("Invalid new packet length/count")
        records = [RECORD.unpack_from(packet, HEADER.size + i * RECORD.size) for i in range(count)]
        description = f"new seq={sequence} version={version} type={data_type}"
    elif packet[:4] == b"\xfa\xfb\xfa\xfb":
        if len(packet) < 24:
            raise ValueError("Truncated legacy header")
        counts, count = packet[4:20], packet[20]
        if sum(counts) != count or len(packet) < 24 + count * 4:
            raise ValueError("Invalid legacy packet length/count")
        records = []
        offset = 24
        for channel, channel_count in enumerate(counts, 1):
            for sensor in range(1, channel_count + 1):
                records.append((channel, sensor, struct.unpack_from("<f", packet, offset)[0]))
                offset += 4
        description = "legacy"
    else:
        raise ValueError("Unknown frame header")
    validate_records(records)
    return description, sorted(records)


def record_argument(value):
    try:
        channel, sensor, wavelength = value.split(":")
        record = (int(channel), int(sensor), float(wavelength))
        validate_records([record])
        return record
    except (ValueError, OverflowError) as error:
        raise argparse.ArgumentTypeError(f"Use CHANNEL:SENSOR:WAVELENGTH: {error}") from error


def ipv4_argument(value):
    try:
        return str(ipaddress.IPv4Address(value))
    except ipaddress.AddressValueError as error:
        raise argparse.ArgumentTypeError("Use an IPv4 address") from error


def print_records(prefix, records):
    sample = ", ".join(f"CH{ch}_{sensor}={wave:.5f}" for ch, sensor, wave in records[:8])
    suffix = " ..." if len(records) > 8 else ""
    print(f"{prefix} records={len(records)} {sample}{suffix}", flush=True)


def send(args, records):
    kind = socket.SOCK_DGRAM if args.transport == "udp" else socket.SOCK_STREAM
    with socket.socket(socket.AF_INET, kind) as connection:
        connection.settimeout(5)
        connection.bind((args.bind, args.source_port))
        target = (args.host, args.port)
        if args.transport == "tcp":
            connection.connect(target)
        print(f"{args.transport.upper()} {connection.getsockname()} -> {target}; Ctrl+C to stop", flush=True)
        interval = args.interval_ms / 1000
        started = time.monotonic()
        index = 0
        while args.count == 0 or index < args.count:
            delta = args.amplitude * math.sin(2 * math.pi * index * interval / 5)
            current = [(ch, sensor, wave + delta) for ch, sensor, wave in records]
            protocol = args.protocol
            if protocol == "alternate":
                protocol = "new" if index % 2 == 0 else "legacy"
            if args.shuffle and protocol == "new":
                random.shuffle(current)
            sequence = (args.sequence + index) & 0xFFFFFFFF
            packet = build_packet(current, protocol, sequence, args.version, args.data_type)
            if args.transport == "udp":
                connection.sendto(packet, target)
            else:
                connection.sendall(packet)
            print_records(f"TX #{index + 1} {protocol} seq={sequence} bytes={len(packet)}", current)
            index += 1
            if args.count == 0 or index < args.count:
                time.sleep(max(0, started + index * interval - time.monotonic()))
        print(f"Sent {index} frames. UDP send success alone does not confirm reception.")


def receive(args):
    # A diagnostic UDP receiver; the application should normally receive instead.
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as connection:
        connection.bind((args.host, args.port))
        print(f"UDP listening at {connection.getsockname()}; Ctrl+C to stop", flush=True)
        received = 0
        while args.count == 0 or received < args.count:
            packet, sender = connection.recvfrom(65535)
            received += 1
            try:
                description, records = decode_packet(packet)
                print_records(f"RX #{received} from={sender} {description} bytes={len(packet)}", records)
            except ValueError as error:
                print(f"RX #{received} from={sender} rejected: {error}", flush=True)


def self_test(source, target):
    # Independent wire fixture: 1550.25 = 0x44C1C800 (IEEE754 float32).
    records = [(1, 2, 1550.25)]
    expected = bytes.fromhex("5aa501020100efcdab89010200c8c144")
    if build_packet(records, sequence=0x89ABCDEF) != expected:
        raise ValueError("New protocol wire fixture failed")
    legacy = build_packet([(1, 1, 1550.25)], "legacy")
    if legacy != bytes.fromhex("fafbfafb01" + "00" * 15 + "0100000000c8c144"):
        raise ValueError("Legacy wire fixture failed")
    modern = build_packet([(2, 1, 1551.5), (1, 1, 1550.25)])
    for packet in (expected, legacy, modern):
        decode_packet(packet)
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as receiver, socket.socket(
        socket.AF_INET, socket.SOCK_DGRAM
    ) as sender:
        receiver.bind((target, 0))
        receiver.settimeout(3)
        sender.bind((source, 0))
        for packet in (expected, legacy, modern):
            sender.sendto(packet, receiver.getsockname())
            actual, address = receiver.recvfrom(65535)
            if actual != packet or address[0] != source:
                raise ValueError("UDP round-trip/source address mismatch")
            decode_packet(actual)
        print(f"PASS UDP adapter: {sender.getsockname()} -> {receiver.getsockname()}")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server, socket.socket(
        socket.AF_INET, socket.SOCK_STREAM
    ) as client:
        server.bind((target, 0))
        server.listen(1)
        server.settimeout(3)
        client.settimeout(3)
        client.bind((source, 0))
        client.connect(server.getsockname())
        peer, address = server.accept()
        with peer:
            peer.settimeout(3)
            client.sendall(modern)
            actual = bytearray()
            while len(actual) < len(modern):
                chunk = peer.recv(len(modern) - len(actual))
                if not chunk:
                    raise ValueError("TCP connection closed before frame completion")
                actual.extend(chunk)
            if bytes(actual) != modern or address[0] != source:
                raise ValueError("TCP round-trip/source address mismatch")
            decode_packet(actual)
        print(f"PASS TCP adapter: {client.getsockname()} -> {server.getsockname()}")
    print("PASS new/legacy wire format, decoding and adapter UDP/TCP round-trips")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", type=ipv4_argument, default=DEFAULT_TARGET, help="Receiver IPv4 (default: %(default)s)")
    parser.add_argument("--bind", type=ipv4_argument, default=DEFAULT_SOURCE, help="Sender adapter IPv4 (default: %(default)s)")
    parser.add_argument("--port", type=int, default=5060, help="Receiver port (default: %(default)s)")
    parser.add_argument("--source-port", type=int, default=0, help="Sender port; 0 selects a free port")
    parser.add_argument("--mode", choices=("send", "receive"), default="send")
    parser.add_argument("--transport", choices=("udp", "tcp"), default="udp")
    parser.add_argument("--protocol", choices=("new", "legacy", "alternate"), default="new")
    parser.add_argument("--count", type=int, default=0, help="Frames; 0 runs until Ctrl+C")
    parser.add_argument("--interval-ms", type=float, default=100, help="Send interval in milliseconds")
    parser.add_argument("--record", type=record_argument, action="append", help="Repeat CHANNEL:SENSOR:WAVELENGTH; overrides default six sensors")
    parser.add_argument("--amplitude", type=float, default=0.05, help="5-second sine wave amplitude; 0 sends constant values")
    parser.add_argument("--sequence", type=int, default=0)
    parser.add_argument("--version", type=int, default=1)
    parser.add_argument("--data-type", type=int, default=2)
    parser.add_argument("--shuffle", action="store_true", help="Shuffle new packet records to test ID mapping")
    parser.add_argument("--self-test", action="store_true", help="Verify wire bytes and real adapter UDP/TCP reception using free ports, then exit")
    args = parser.parse_args()
    if not 1 <= args.port <= 65535 or not 0 <= args.source_port <= 65535:
        parser.error("Invalid port (receiver 1..65535, source 0..65535)")
    if args.count < 0 or not math.isfinite(args.interval_ms) or args.interval_ms <= 0:
        parser.error("count must be nonnegative and interval-ms must be finite and positive")
    if not math.isfinite(args.amplitude) or args.amplitude < 0:
        parser.error("amplitude must be finite and nonnegative")
    if not 0 <= args.version <= 255 or not 0 <= args.data_type <= 255:
        parser.error("version/data-type must be 0..255")
    if args.mode == "receive" and args.transport != "udp":
        parser.error("Diagnostic receive mode supports UDP; use the FOSD application as TCP receiver")
    records = args.record or [(ch, sensor, 1550 + (ch - 1) * 5 + (sensor - 1) * 0.25)
                              for ch in range(1, 3) for sensor in range(1, 4)]
    try:
        if args.self_test:
            self_test(args.bind, args.host)
        elif args.mode == "receive":
            receive(args)
        else:
            # Validate all requested packet formats before transmitting any data.
            for protocol in (("new", "legacy") if args.protocol == "alternate" else (args.protocol,)):
                build_packet(records, protocol, args.sequence, args.version, args.data_type)
            send(args, records)
    except KeyboardInterrupt:
        print("\nStopped.")
    except (OSError, ValueError, OverflowError, struct.error) as error:
        print(f"Error: {error}\nCheck the adapter IPv4, receiver IP/port and whether reception is enabled.", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
