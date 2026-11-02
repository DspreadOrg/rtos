#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_OUT="$PROJECT_ROOT/out/A30R_debug"

export PROJECT_ROOT
export PROJECT_OUT
export BUILD_TARGET=A30R
export BUILD_RELEASE_TYPE=debug
export MY_PRJ="-DHW_A30R -DXML_MENU"

export PATH="$PROJECT_ROOT/tools:$PROJECT_ROOT/tools/linux:$PATH"
export LD_LIBRARY_PATH="$PROJECT_ROOT/tools/linux/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

for command in arm-none-eabi-gcc arm-none-eabi-g++ cmake ninja python3 dtools; do
    if ! command -v "$command" >/dev/null 2>&1; then
        printf 'Required command not found in PATH: %s\n' "$command" >&2
        exit 1
    fi
done

for library in \
    "$PROJECT_ROOT/examples/Demo/Lib/libSelfCheck_A30R.a" \
    "$PROJECT_ROOT/examples/Demo/Lib/libSDK_A30R.a"; do
    if [[ ! -f "$library" ]]; then
        printf 'Required library does not exist: %s\n' "$library" >&2
        exit 1
    fi
done

cmake -S "$PROJECT_ROOT" -B "$PROJECT_OUT" -G Ninja
cmake --build "$PROJECT_OUT" --parallel

HEX_DIR="$PROJECT_OUT/hex"
VERSION_OBJECT="$PROJECT_OUT/examples/Demo/CMakeFiles/mainapp_flash.dir/Application/Source/main.c.obj"
IMAGE="$HEX_DIR/mainapp_flash.img"
PAC="$HEX_DIR/mainapp_flash.pac"

if [[ ! -f "$VERSION_OBJECT" ]]; then
    printf 'Version object does not exist: %s\n' "$VERSION_OBJECT" >&2
    exit 1
fi

if [[ ! -f "$IMAGE" ]]; then
    printf 'Build image does not exist: %s\n' "$IMAGE" >&2
    exit 1
fi

firmware_version="$(python3 - "$VERSION_OBJECT" <<'PY'
from pathlib import Path
import re
import sys

data = Path(sys.argv[1]).read_bytes()
match = re.search(rb"VER:(V[A-Za-z0-9._+-]+)", data)
if match is None:
    print(f"Firmware version marker not found in {sys.argv[1]}", file=sys.stderr)
    sys.exit(1)
print(match.group(1).decode("ascii"))
PY
)"

RENAMED_IMAGE="$HEX_DIR/${BUILD_TARGET}_APP_${firmware_version}.img"
cp -- "$IMAGE" "$RENAMED_IMAGE"
rm -- "$PAC"

printf 'Build completed. Renamed image: %s\n' "$RENAMED_IMAGE"
