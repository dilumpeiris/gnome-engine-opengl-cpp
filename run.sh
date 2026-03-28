#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

# Create build dir if needed
mkdir -p "$BUILD_DIR"

# Configure if CMakeCache doesn't exist yet
if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
    echo ">> Configuring CMake..."
    cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR"
fi

# Build
echo ">> Building..."
cmake --build "$BUILD_DIR" -- -j$(sysctl -n hw.logicalcpu)

# Run
echo ">> Running GnomeEngine..."
cd "$BUILD_DIR"
./bin/GnomeEngine
