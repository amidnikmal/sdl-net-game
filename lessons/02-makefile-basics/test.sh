#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="${1:-solution}"
MAKE_CMD="${MAKE:-make}"
COMPILER="${CC:-gcc}"
EXPECTED="2 + 3 = 5"

if ! command -v "$MAKE_CMD" >/dev/null 2>&1; then
  echo "FAIL: make command '$MAKE_CMD' not found"
  echo "Set MAKE=/path/to/make if needed."
  exit 1
fi

if ! command -v "$COMPILER" >/dev/null 2>&1; then
  echo "FAIL: compiler '$COMPILER' not found"
  echo "Set CC=/path/to/compiler if gcc is unavailable."
  exit 1
fi

"$MAKE_CMD" -C "$PROJECT_DIR" clean >/dev/null 2>&1 || true
"$MAKE_CMD" -C "$PROJECT_DIR" CC="$COMPILER"

OUTPUT="$("$PROJECT_DIR"/app)"

if [[ "$OUTPUT" != "$EXPECTED" ]]; then
  echo "FAIL: unexpected output"
  echo "expected: $EXPECTED"
  echo "actual:   $OUTPUT"
  exit 1
fi

if [[ ! -f "$PROJECT_DIR/build/main.o" || ! -f "$PROJECT_DIR/build/add.o" ]]; then
  echo "FAIL: expected object files were not created"
  exit 1
fi

"$MAKE_CMD" -C "$PROJECT_DIR" clean >/dev/null

if [[ -e "$PROJECT_DIR/app" || -e "$PROJECT_DIR/build" ]]; then
  echo "FAIL: make clean did not remove build artifacts"
  exit 1
fi

echo "OK: $PROJECT_DIR"
echo "output: $OUTPUT"
