#!/usr/bin/env bash
set -euo pipefail

SOURCE="${1:-solution/main.c}"
COMPILER="${CC:-gcc}"
TMPDIR="$(mktemp -d)"
BIN="$TMPDIR/lesson04"
EXPECTED=$'hex: 50 4f 53 00 03 01 2c 02 bc 07\ntag: POS\ntype: 3\nx: 300\ny: 700\ndimension: 7'

cleanup() {
  rm -rf "$TMPDIR"
}

trap cleanup EXIT

if ! command -v "$COMPILER" >/dev/null 2>&1; then
  echo "FAIL: compiler '$COMPILER' not found"
  echo "Set CC=/path/to/compiler if gcc is unavailable."
  exit 1
fi

"$COMPILER" -Wall -Wextra -g "$SOURCE" -o "$BIN"
OUTPUT="$("$BIN")"

if [[ "$OUTPUT" != "$EXPECTED" ]]; then
  echo "FAIL: unexpected output"
  echo "expected:"
  printf '%s\n' "$EXPECTED"
  echo "actual:"
  printf '%s\n' "$OUTPUT"
  exit 1
fi

echo "OK: $SOURCE"
printf 'output:\n%s\n' "$OUTPUT"
