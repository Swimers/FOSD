#!/usr/bin/env python3
"""Check the Git index, or all reachable history, for private-data paths.

This checks repository boundaries, not the confidentiality of arbitrary source text.
"""
import argparse
import re
import subprocess
from pathlib import Path


def forbidden(path):
    normalized = path.replace("\\", "/").lower()
    # Only the repository's dataset root is private. Source features may also
    # legitimately be named "data" (for example ui/.../features/data).
    if normalized.startswith("data/") and not (
        normalized == "data/qss" or normalized.startswith("data/qss/")
    ):
        return True
    return normalized.startswith(("build/", "bulid/")) or bool(
        re.fullmatch(r"release/fosd-.*\.zip(?:\.sha256)?", normalized)
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--history", action="store_true", help="Check all reachable Git refs")
    arguments = parser.parse_args()
    command = ["git", "-C", str(arguments.repo)]
    if arguments.history:
        output = subprocess.check_output(command + ["rev-list", "--objects", "--all"])
        paths = [row.partition(b" ")[2] for row in output.splitlines() if b" " in row]
    else:
        output = subprocess.check_output(command + ["ls-files", "-z"])
        paths = [path for path in output.split(b"\0") if path]
    bad = sorted({path.decode("utf-8", "surrogateescape") for path in paths
                  if forbidden(path.decode("utf-8", "surrogateescape"))})
    if bad:
        print("Private-data boundary check FAILED: %d paths" % len(bad))
        for path in bad[:20]:
            print("  " + ascii(path))
        return 1
    print("Private-data boundary check passed (%s)." %
          ("all reachable history" if arguments.history else "index"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
