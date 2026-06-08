#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="${1:-solution}"
COMPILER="${CC:-gcc}"
TMPDIR="$(mktemp -d)"
BIN="$TMPDIR/atom02"
EXPECTED='value: 3'

cleanup() {
  rm -rf "$TMPDIR"
}

trap cleanup EXIT

if ! command -v "$COMPILER" >/dev/null 2>&1; then
  echo "FAIL: compiler '$COMPILER' not found"
  echo "Set CC=/path/to/compiler if gcc is unavailable."
  exit 1
fi

"$COMPILER" -Wall -Wextra -g \
  "$PROJECT_DIR/main.c" \
  "$PROJECT_DIR/counter.c" \
  -o "$BIN"

OUTPUT="$("$BIN")"

if [[ "$OUTPUT" != "$EXPECTED" ]]; then
  echo "FAIL: unexpected output"
  echo "expected: $EXPECTED"
  echo "actual:   $OUTPUT"
  exit 1
fi

echo "OK: $PROJECT_DIR"
