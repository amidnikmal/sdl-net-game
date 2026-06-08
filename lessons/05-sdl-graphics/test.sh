#!/usr/bin/env bash
set -euo pipefail

SOURCE="${1:-solution/main.c}"
COMPILER="${CC:-gcc}"
TMPDIR="$(mktemp -d)"
BIN="$TMPDIR/lesson05"
EXPECTED=$'inside: 255 0 0\noutside: 255 255 255'

cleanup() {
  rm -rf "$TMPDIR"
}

trap cleanup EXIT

if ! command -v "$COMPILER" >/dev/null 2>&1; then
  echo "FAIL: compiler '$COMPILER' not found"
  echo "Set CC=/path/to/compiler if gcc is unavailable."
  exit 1
fi

if command -v sdl-config >/dev/null 2>&1; then
  read -r -a SDL_FLAGS <<< "$(sdl-config --cflags --libs)"
elif command -v pkg-config >/dev/null 2>&1 && pkg-config --exists sdl; then
  read -r -a SDL_FLAGS <<< "$(pkg-config --cflags --libs sdl)"
else
  echo "FAIL: SDL build flags not found"
  echo "Need SDL 1.2 development package with sdl-config or pkg-config metadata."
  exit 1
fi

"$COMPILER" -Wall -Wextra -g "$SOURCE" -o "$BIN" "${SDL_FLAGS[@]}"
OUTPUT="$(SDL_VIDEODRIVER=dummy "$BIN")"

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
