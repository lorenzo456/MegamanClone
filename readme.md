# MegaMan Clone - Dreamcast Port

## Overview

- Personal project creating a C++ game without a game engine
- Small Megaman-inspired platformer with 2 enemy types and a boss battle
- Successfully ported to Sega Dreamcast retro console

## Technologies

- **Raylib4Dreamcast** - Raylib port for Dreamcast
- **DreamSDK** - Dreamcast development toolchain
- **KallistiOS (KOS)** - Dreamcast operating system library
- Huge thanks to [Niisoks](https://github.com/Niisoks) for the raylib examples in KallistiOS

## Prerequisites

- DreamSDK installed and configured
- KallistiOS environment set up
- Raylib4Dreamcast library in KOS ports

## Project Structure

```
megamanclone/
├── src/
│   ├── main/
│   ├── player/
│   ├── enemy/
│   ├── boss/
│   ├── bullet/
│   ├── obstacle/
│   └── sound/
├── romdisk/
│   └── assets/
│       ├── Sprites/     
│       └── sound/      
└── Makefile

```

## Building

### Development Build

```bash
# Clean previous build
make clean

# Build ELF for emulator testing
make

# Test in Flycast emulator
flycast megamanclone.elf 

or

Drag and drop the cdi into any emulator you may have

```

**Important:** If it works in Flycast, it should work on real hardware when burned correctly.

### Create Bootable Disc

```bash
# Build CDI for real Dreamcast hardware
make clean
make dist
```

This creates:

- `megamanclone.elf` - Game executable
- `1ST_READ.BIN` - Scrambled binary
- `IP.BIN` - Boot sector metadata
- `megaman.iso` - Disc image
- `megaman.cdi` - Bootable Dreamcast disc image

### Verify Boot Sector (Optional)

```bash
# Check if IP.BIN is properly embedded in ISO
dd if=megaman.iso bs=1 count=200 2>/dev/null | strings | head -20

```

Should display "SEGA SEGAKATANA" and game title.

## Burning to CD-R

### Requirements

- **CD-R Media:** Verbatim brand (700MB) recommended
- **Burning Software:** ImgBurn (Windows)
- **Burn Speed:** 4x (critical - faster speeds often fail!)

### ImgBurn Settings

1. Select "Write image file to disc"
2. Choose `megaman.cdi`
3. Set write speed to **4x** (not 8x, not auto)
4. Enable "Verify after write"
5. Enable "Finalize Disc"
6. Burn and wait for verification to complete

## License

Personal project - feel free to use as reference for your own Dreamcast homebrew!
