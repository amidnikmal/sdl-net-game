#!/usr/bin/env bash
set -euo pipefail

SOURCE="${1:-solution/main.c}"
COMPILER="${CC:-gcc}"
TMPDIR="$(mktemp -d)"
BIN="$TMPDIR/lesson03"
EXPECTED=$'server received: ping\nclient received: pong'

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

if command -v timeout >/dev/null 2>&1; then
  OUTPUT="$(timeout 3 "$BIN")"
else
  OUTPUT="$("$BIN")"
fi

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
