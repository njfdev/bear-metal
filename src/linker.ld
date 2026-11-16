ENTRY(_start)

SECTIONS {

  . = 2M; /* 2M is a safe start offset */

  /* Multiboot header and code */
  .text BLOCK(4K) : ALIGN(4K) {
    *(.multiboot)
    *(.text)
  }

  /* read only data */
  .rodata BLOCK(4K) : ALIGN(4k) {
    *(.rodata)
  }

  /* initialized read-write data */
  .data BLOCK(4K) : ALIGN(4K) {
    *(.data)
  }

  /* uninitialized read-write data */
  .bss BLOCK(4K) : ALIGN(4K) {
    *(COMMON)
    *(.bss)
  }

}