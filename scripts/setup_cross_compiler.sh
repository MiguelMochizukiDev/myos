#!/usr/bin/env bash

set -e

TARGET="x86_64-elf"
PREFIX="$HOME/opt/cross"

BINUTILS_VERSION="2.42"
GCC_VERSION="14.1.0"

BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz"
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz"

JOBS=$(nproc)
BUILD_DIR="/tmp/myos-cross-build"

echo "==> Target  : $TARGET"
echo "==> Prefix  : $PREFIX"
echo "==> Binutils: $BINUTILS_VERSION"
echo "==> GCC     : $GCC_VERSION"
echo "==> Jobs    : $JOBS"
echo ""

mkdir -p "$PREFIX"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "==> Installing build dependencies..."
sudo apt-get update -qq
sudo apt-get install -y \
    wget tar \
    gcc g++ make \
    bison flex \
    libgmp-dev libmpfr-dev libmpc-dev \
    texinfo

echo "==> Downloading sources..."

if [ ! -f "binutils-${BINUTILS_VERSION}.tar.xz" ]; then
    wget -q --show-progress "$BINUTILS_URL"
fi

if [ ! -f "gcc-${GCC_VERSION}.tar.xz" ]; then
    wget -q --show-progress "$GCC_URL"
fi

echo "==> Extracting sources..."
tar -xf "binutils-${BINUTILS_VERSION}.tar.xz"
tar -xf "gcc-${GCC_VERSION}.tar.xz"

echo "==> Building binutils..."
mkdir -p "build-binutils"
cd "build-binutils"
"../binutils-${BINUTILS_VERSION}/configure" \
    --target="$TARGET" \
    --prefix="$PREFIX" \
    --with-sysroot \
    --disable-nls \
    --disable-werror \
    --quiet
make -j"$JOBS"
make install
cd ..

echo "==> Building GCC (C only, freestanding)..."
mkdir -p "build-gcc"
cd "build-gcc"
"../gcc-${GCC_VERSION}/configure" \
    --target="$TARGET" \
    --prefix="$PREFIX" \
    --disable-nls \
    --enable-languages=c \
    --without-headers \
    --quiet
make -j"$JOBS" all-gcc
make -j"$JOBS" all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..

echo ""
echo "==> Done. Add the following to your shell profile:"
echo ""
echo "    export PATH=\"\$PATH:$PREFIX/bin\""
echo ""
echo "==> Then verify with:"
echo ""
echo "    ${TARGET}-gcc --version"
echo "    ${TARGET}-ld  --version"
