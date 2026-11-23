#include <descriptors.h>

void init_descriptors(gdt_t gdt[256], idt_t idt[256]) {
	// setup GDT for data segment
  set_gdt(gdt+1, 0b10010010);

	// setup GDT for code segment
  set_gdt(gdt+2, 0b10011010);

  segment_info_t gdt_info = {
    .limit = 0x7ff,
    .base = (uint32_t) gdt
  };

  segment_info_t idt_info = {
    .limit = 0x7ff,
    .base = (uint32_t) idt
  };

  // load info about descriptor tables into registers
  __asm__ volatile ("lgdt %0"
		:
		: "m" (gdt_info)
		:
	);
  __asm__ volatile ("lidt %0"
		:
		: "m" (idt_info)
		:
	);

}

void set_gdt(gdt_t *gdt, uint8_t access) {
	*gdt = (gdt_t) {
		.limit_lower = 0xffff,
		.base_lower = 0,
		.base_middle = 0,
		.access = access,
		.limit_upper_and_flags = (0xc0) + 0xf,
		.base_upper = 0
	};
}

void set_idt(idt_t *idt, void handler_entry(void)) {
	*idt = (idt_t) {
		.offset_low = (uint32_t)handler_entry & 0xffff,
		.selector = 0x10,	// points to GDP entry 2 (the text section)
		.attributes = 0x8e,	// interrupt gate type
		.offset_high = (uint32_t)handler_entry >> 16,
	};
}