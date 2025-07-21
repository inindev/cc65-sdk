#!/bin/sh

set -e

# check if running on linux
if [ "$(uname -s)" != "Linux" ]; then
    echo "Error: This script requires Linux."
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
ARCH=$(dpkg --print-architecture)

# create debian directory structure
rm -rfv stage/deb
mkdir -pv stage/deb/DEBIAN \
          stage/deb/usr/bin \
          stage/deb/usr/share/cc65-sdk/asminc \
          stage/deb/usr/share/cc65-sdk/cfg \
          stage/deb/usr/share/cc65-sdk/include \
          stage/deb/usr/share/cc65-sdk/lib \
          stage/deb/usr/share/cc65-sdk/samples \
          stage/deb/usr/share/cc65-sdk/target \
          stage/deb/usr/share/doc/cc65-sdk

# copy files with correct permissions
install -vm 755 bin/* stage/deb/usr/bin/
install -vm 644 asminc/* stage/deb/usr/share/cc65-sdk/asminc/ 2>/dev/null || true
install -vm 644 cfg/* stage/deb/usr/share/cc65-sdk/cfg/ 2>/dev/null || true
install -vm 644 include/* stage/deb/usr/share/cc65-sdk/include/ 2>/dev/null || true
install -vm 644 lib/* stage/deb/usr/share/cc65-sdk/lib/ 2>/dev/null || true
install -vm 644 samples/* stage/deb/usr/share/cc65-sdk/samples/ 2>/dev/null || true
install -vm 644 target/* stage/deb/usr/share/cc65-sdk/target/ 2>/dev/null || true
install -vm 644 doc/* stage/deb/usr/share/doc/cc65-sdk/ 2>/dev/null || true

# create control file
cat > stage/deb/DEBIAN/control << EOF
Package: cc65-sdk
Version: $VERSION-1
Architecture: $ARCH
Maintainer: Laszlo Boszormenyi (GCS) <gcs@debian.org>
Installed-Size: $(du -sk stage/deb/usr | cut -f1)
Depends: libc6 (>= 2.29)
Suggests: cc65-sdk-doc
Section: otherosfs
Priority: optional
Homepage: https://cc65.github.io/cc65/
Description: complete cross development package for 65(C)02 systems
 cc65 is a complete cross development package for 65(C)02 systems, including
 a powerful macro assembler, a C compiler, linker, librarian and several
 other tools.
 .
 cc65 has C and runtime library support for many of the old 6502 machines,
 including
   - the following Commodore machines:
     - VIC20
     - C16/C116 and Plus/4
     - C64
     - C128
     - CBM 510 (aka P500)
     - the 600/700 family
     - newer PET machines (not 2001).
   - the Apple ][+ and successors.
   - the Atari 8 bit machines.
   - the Atari 5200 console.
   - GEOS for the C64, C128 and Apple //e.
   - the Nintendo Entertainment System (NES) console.
   - the Supervision console.
   - the Oric Atmos.
   - the Lynx console.
   - the Ohio Scientific Challenger 1P
EOF

# create copyright file
cat > stage/deb/DEBIAN/copyright << 'EOF'
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: cc65
Source: https://github.com/cc65/cc65

Files: *
Copyright: 1998-2025 Ullrich von Bassewitz and others
License: zlib
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from
 the use of this software.
 .
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 .
 1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software in
    a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
EOF

# create postinst script
cat > stage/deb/DEBIAN/postinst << 'EOF'
#!/bin/sh
echo
echo "Installation successful"
echo
echo "To use the cc65-sdk, add the following to your shell configuration (e.g., ~/.bashrc):"
echo "  export CC65_HOME=/usr/share/cc65-sdk"
echo
echo "To uninstall later, run:"
echo "  sudo apt remove cc65-sdk"
EOF
chmod 755 stage/deb/DEBIAN/postinst

# build the .deb package
dpkg-deb --build stage/deb stage/deb/cc65-sdk-${VERSION}-1-linux-${ARCH}.deb

echo "stage/deb/cc65-sdk-${VERSION}-1-linux-${ARCH}.deb created successfully."
