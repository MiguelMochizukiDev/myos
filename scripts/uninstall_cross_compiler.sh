#!/usr/bin/env bash

set -e

TARGET="x86_64-elf"
PREFIX="$HOME/opt/cross"
BUILD_DIR="/tmp/myos-cross-build"

echo "==> Target : $TARGET"
echo "==> Prefix : $PREFIX"
echo ""

if [ ! -d "$PREFIX" ]; then
    echo "==> Nothing to remove: $PREFIX does not exist."
    exit 0
fi

read -rp "==> Remove $PREFIX and build artifacts? [y/N] " confirm
if [ "$confirm" != "y" ] && [ "$confirm" != "Y" ]; then
    echo "==> Aborted."
    exit 0
fi

echo "==> Removing $PREFIX..."
rm -rf "$PREFIX"

if [ -d "$BUILD_DIR" ]; then
    echo "==> Removing build directory $BUILD_DIR..."
    rm -rf "$BUILD_DIR"
fi

echo ""
echo "==> Done. Remember to remove the PATH entry from your shell profile:"
echo ""
echo "    export PATH=\"\$PATH:$PREFIX/bin\""
