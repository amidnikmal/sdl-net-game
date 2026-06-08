#!/usr/bin/env bash
set -euo pipefail

SOURCE="${1:-solution/main.c}"
TMPDIR="$(mktemp -d)"
OBJ="$TMPDIR/lesson01.o"
BIN="$TMPDIR/lesson01"
EXPECTED="2 + 3 = 5"
COMPILER="${CC:-gcc}"

cleanup() {
  rm -rf "$TMPDIR"
}

trap cleanup EXIT

if ! command -v "$COMPILER" >/dev/null 2>&1; then
  echo "FAIL: compiler '$COMPILER' not found"
  echo "Set CC=/path/to/compiler if gcc is unavailable."
  exit 1
fi

"$COMPILER" -Wall -Wextra -g -c "$SOURCE" -o "$OBJ"
"$COMPILER" -Wall -Wextra -g "$OBJ" -o "$BIN"

OUTPUT="$("$BIN")"

if [[ "$OUTPUT" != "$EXPECTED" ]]; then
  echo "FAIL: unexpected output"
  echo "expected: $EXPECTED"
  echo "actual:   $OUTPUT"
  exit 1
fi

echo "OK: $SOURCE"
echo "output: $OUTPUT"
