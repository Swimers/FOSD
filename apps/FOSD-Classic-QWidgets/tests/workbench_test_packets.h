#pragma once
#include <QByteArray>
#include <QtEndian>
#include <cstring>
namespace workbench_test {
inline QByteArray packet(quint32 sequence) {
    QByteArray bytes(10, '\0');
    bytes[0] = char(0x5a);
    bytes[1] = char(0xa5);
    bytes[2] = 1;
    bytes[3] = 1;
    qToLittleEndian<quint16>(12, reinterpret_cast<uchar *>(bytes.data() + 4));
    qToLittleEndian(sequence, reinterpret_cast<uchar *>(bytes.data() + 6));
    for (int i = 0; i < 12; ++i) {
        bytes.append(char(i + 1));
        bytes.append(char(1));
        const float value = float(1550.0 + 0.5 * i - 0.04);
        quint32 bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));
        char encoded[4];
        qToLittleEndian(bits, reinterpret_cast<uchar *>(encoded));
        bytes.append(encoded, 4);
    }
    return bytes;
}
} // namespace workbench_test
