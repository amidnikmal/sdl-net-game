#!/usr/bin/env bash
set -euo pipefail

SOURCE="${1:-solution/main.c}"
COMPILER="${CC:-gcc}"
TMPDIR="$(mktemp -d)"
BIN="$TMPDIR/lesson06"
EXPECTED=$'connect alice: sync=0 broadcast=1 total=1\nconnect bob: sync=1 broadcast=2 total=2\nposition alice: forward=1'

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
