QEMU := qemu-system-x86_64
QEMU_ARGS := -m 512M

TARGET := bear-metal
SRC := src
BUILD := build

all: $(BUILD)/$(TARGET).iso

run: all
	$(QEMU) $(QEMU_ARGS) -cdrom $(BUILD)/$(TARGET).iso

$(BUILD)/$(TARGET).iso: $(BUILD) $(BUILD)/boot.bin
	mkdir -p $(BUILD)/iso

	dd if=/dev/zero of=$(BUILD)/iso/$(TARGET).img bs=1024 count=1440
	dd if=$(BUILD)/boot.bin of=$(BUILD)/iso/$(TARGET).img conv=notrunc

	xorriso -as mkisofs -b $(TARGET).img -o $(BUILD)/$(TARGET).iso $(BUILD)/iso

$(BUILD)/boot.bin: $(SRC)/boot.asm
	nasm $(SRC)/boot.asm -o $(BUILD)/boot.bin

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -r $(BUILD)