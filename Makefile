TARGET := bear-metal
SRC := src
BUILD := build
OBJ := $(BUILD)/obj

QEMU := qemu-system-x86_64
QEMU_ARGS := -m 512M -drive if=pflash,format=raw,readonly=on,file="$(BUILD)/OVMF.fd"

SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

CC := i686-elf-gcc
CFLAGS := -T $(SRC)/linker.ld -ffreestanding -nostdlib -lgcc -O2

GRUB_PREFIX := x86_64-elf-grub

all: $(BUILD) $(BUILD)/$(TARGET).iso

run: all | $(BUILD)/OVMF.fd
	$(QEMU) $(QEMU_ARGS) -cdrom $(BUILD)/$(TARGET).iso

$(BUILD)/OVMF.fd:
	# OVMF UEFI bios download
	curl -L -o $(BUILD)/OVMF.fd https://github.com/clearlinux/common/raw/f98964d9b91484006a888db4c3521425a049699b/OVMF.fd

$(BUILD)/$(TARGET).iso: $(OBJ)/boot.o $(OBJS)
	mkdir -p $(BUILD)/iso/boot/grub

	$(CC) $(CFLAGS) $^ -o $(BUILD)/iso/boot/$(TARGET)

	cp $(SRC)/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg

	$(GRUB_PREFIX)-mkrescue -o $(BUILD)/$(TARGET).iso $(BUILD)/iso

$(OBJ)/boot.o: $(SRC)/boot.asm | $(OBJ)
	nasm -f elf32 $(SRC)/boot.asm -o $(OBJ)/boot.o

$(OBJS): $(SRCS) | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ): $(BUILD)
	mkdir -p $(OBJ)

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -r $(BUILD)