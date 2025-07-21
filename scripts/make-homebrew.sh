#!/bin/sh

set -e

# check if running on macOS
if [ "$(uname -s)" != "Darwin" ]; then
    echo "Error: This script requires macOS."
    exit 1
fi

# check if source directories exist
if [ ! -d "bin" ] || [ ! -d "asminc" ] || [ ! -d "cfg" ] || [ ! -d "include" ] || [ ! -d "lib" ] || [ ! -d "samples" ] || [ ! -d "target" ]; then
    echo "Error: Required directories (bin, asminc, cfg, include, lib, samples, target) not found."
    exit 1
fi

# capture version
VERSION=$(bin/cc65 --version 2>&1 | sed 's/.*V\([^ ]*\).*/\1/')
if [ -z "$VERSION" ]; then
    echo "Error: Could not determine cc65 version."
    exit 1
fi

# capture architecture
ARCH=$(uname -m)
if [ "$ARCH" = "aarch64" ]; then
    ARCH="arm64"
fi

# create homebrew staging directory structure
rm -rf stage/homebrew
mkdir -p stage/homebrew/cc65-sdk/$VERSION/bin \
         stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/asminc \
         stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/cfg \
         stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/include \
         stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/lib \
         stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/samples \
         stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/target \
         stage/homebrew/cc65-sdk/$VERSION/share/doc/cc65-sdk

# copy files with correct permissions
install -vm 755 bin/* stage/homebrew/cc65-sdk/$VERSION/bin/
install -vm 644 asminc/* stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/asminc/ 2>/dev/null || true
install -vm 644 cfg/* stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/cfg/ 2>/dev/null || true
install -vm 644 include/* stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/include/ 2>/dev/null || true
install -vm 644 lib/* stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/lib/ 2>/dev/null || true
install -vm 644 samples/* stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/samples/ 2>/dev/null || true
install -vm 644 target/* stage/homebrew/cc65-sdk/$VERSION/share/cc65-sdk/target/ 2>/dev/null || true
install -vm 644 doc/* stage/homebrew/cc65-sdk/$VERSION/share/doc/cc65-sdk/ 2>/dev/null || true

# create license file
cat > stage/homebrew/cc65-sdk/$VERSION/share/doc/cc65-sdk/LICENSE << 'EOF'
cc65 is licensed under the Zlib License.

Copyright (c) 1998-2025 Ullrich von Bassewitz and others

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software in
   a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
EOF

# create tarball
tar -czf stage/homebrew/cc65-sdk-$VERSION-macos-$ARCH.tar.gz -C stage/homebrew cc65-sdk

echo "stage/homebrew/cc65-sdk-$VERSION-macos-$ARCH.tar.gz created successfully."
