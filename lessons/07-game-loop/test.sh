#!/usr/bin/env bash
set -euo pipefail

SOURCE="${1:-solution/main.c}"
COMPILER="${CC:-gcc}"
TMPDIR="$(mktemp -d)"
BIN="$TMPDIR/lesson07"
EXPECTED=$'frame 1: net=none input=right update=x=1 send=yes render=x=1\nframe 2: net=left input=none update=x=0 send=no render=x=0\nframe 3: net=none input=up update=x=0 send=no render=x=0'

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
