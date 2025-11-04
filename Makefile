#
# MegaMan Clone - Dreamcast
#

# Target executable
TARGET     = megamanclone.elf
TARGET_BIN = 1ST_READ.BIN
OBJS       = src/boss/boss.o \
             src/bullet/bullet.o \
             src/enemy/enemy.o \
             src/main/main.o \
             src/obstacle/obstacle.o \
             src/player/player.o \
             src/sound/soundManager.o \
             romdisk.o

# KOS settings
KOS_ROMDISK_DIR = romdisk
KOS_CFLAGS += -I${KOS_PORTS}/include/raylib \
              -Isrc/boss \
              -Isrc/bullet \
              -Isrc/enemy \
              -Isrc/main \
              -Isrc/obstacle \
              -Isrc/player \
              -Isrc/sound

# ------------------------
# Default target
all: rm-elf $(TARGET)

# Include KOS rules
include $(KOS_BASE)/Makefile.rules

# ------------------------
# Clean
clean: rm-elf
	-rm -f $(OBJS) romdisk.*
	-rm -f $(TARGET_BIN) megaman.iso megaman.cdi
	-rm -f ip.txt IP.BIN
	-rm -rf cdroot/

rm-elf:
	-rm -f $(TARGET)

# ------------------------
# Build ELF
$(TARGET): $(OBJS)
	kos-c++ -o $(TARGET) $(OBJS) -lraylib -lGL -lwav

# ------------------------
# Run in loader
run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

# ------------------------
# Create binary from ELF
$(TARGET_BIN): $(TARGET)
	$(KOS_OBJCOPY) -R .stack -O binary $(TARGET) $(TARGET_BIN)

# ------------------------
# Scramble binary (makes it bootable)
scramble: $(TARGET_BIN)
	$(KOS_BASE)/utils/scramble/scramble $(TARGET_BIN) $(TARGET_BIN)

# ------------------------
# Create IP.BIN boot sector
ip.txt:
	@echo "Hardware ID   : SEGA SEGAKATANA" > ip.txt
	@echo "Maker ID      : SEGA ENTERPRISES" >> ip.txt
	@echo "Device Info   : CD-ROM1/1" >> ip.txt
	@echo "Area Symbols  : JUE" >> ip.txt
	@echo "Peripherals   : E000F10" >> ip.txt
	@echo "Product No    : T-13337" >> ip.txt
	@echo "Version       : V1.000" >> ip.txt
	@echo "Release Date  : 20241102" >> ip.txt
	@echo "Boot Filename : 1ST_READ.BIN" >> ip.txt
	@echo "SW Maker Name : Lorenzo James" >> ip.txt
	@echo "Game Title    : MEGAMAN CLONE" >> ip.txt

IP.BIN: ip.txt
	$(KOS_BASE)/utils/makeip/makeip ip.txt IP.BIN

# ------------------------
# Create ISO with IP.BIN boot sector
iso: scramble IP.BIN
	@echo "Creating CD root..."
	@mkdir -p cdroot
	@cp $(TARGET_BIN) cdroot/
	@echo "Generating ISO with IP.BIN boot sector..."
	mkisofs -C 0,11702 -V MEGAMAN -G IP.BIN -joliet -rock -l -o megaman.iso cdroot/
	@echo "Verifying IP.BIN is in boot sector..."
	@dd if=megaman.iso bs=1 count=100 2>/dev/null | strings | grep -q "SEGA" && echo "✓ Boot sector OK" || echo "✗ Boot sector FAILED"
	@echo "======================================"
	@echo "ISO created: megaman.iso"
	@echo "Size: $$(du -h megaman.iso | cut -f1)"
	@echo "======================================"

# ------------------------
# Create CDI from ISO
cdi: iso
	@echo "Converting ISO to CDI..."
	$(KOS_BASE)/utils/cdi4dc/cdi4dc megaman.iso megaman.cdi
	@echo "======================================"
	@echo "CDI created: megaman.cdi"
	@echo "Size: $$(du -h megaman.cdi | cut -f1)"
	@echo "Burn at 4x speed with Verbatim CD-R!"
	@echo "======================================"

# ------------------------
# Distribution target
dist: $(TARGET)
	-rm -f $(OBJS) romdisk.img
	$(KOS_STRIP) $(TARGET)
	$(MAKE) cdi