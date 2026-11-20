#include <stdint.h>
#include "frame_buffer_utils.h"
#include "font.h"

struct multiboot_aout_symbol_table
{
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
};

struct multiboot_elf_section_header_table
{
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
};

#include <stdint.h>

typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    // Framebuffer fields (valid if flags bit 12 is set)
    uint32_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint8_t  color_info[6];
} __attribute__((packed)) multiboot_info_t;

void kernel_main(unsigned long magic, multiboot_info_t *info) {
	FrameBuffer fb = {
		.buffer = (uint32_t*)0x80000000,
		.width = 640,
		.height = 480,
		.pitch = 640,
		.red_shift = 16,
		.green_shift = 8,
		.blue_shift = 0,
		.alpha_shift = 24
	};

	Color background = {20, 20, 20, 0};
	Color white = {255, 255, 255, 0};	
	Color black = {0, 0, 0, 0};
	Color red = {255, 0, 0, 0};
	Color green = {0, 255, 0, 0};
	Color blue = {0, 0, 255, 0};

	// set the background to be gray
	clear_screen(&fb, background);

	for (size_t x = 100; x < fb.width - 100; x++) {
		for (size_t y = 40; y < 80; y++) {
			put_pixel(&fb, (Point){x, y}, red);
		}
		for (size_t y = 80; y < 120; y++) {
			put_pixel(&fb, (Point){x, y}, green);
		}
		for (size_t y = 120; y < 160; y++) {
			put_pixel(&fb, (Point){x, y}, blue);	
		}
	}
	
	draw_text(&fb, "Hello World", (Point){200, 200}, white, 1);
}
