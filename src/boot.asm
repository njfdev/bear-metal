%define ALIGN 0b01
%define MEMINFO 0b10
%define FLAGS ALIGN + MEMINFO
%define MAGIC 0x1badb002 ; magic multiboot flag for GRUB
%define CHECKSUM -(MAGIC + FLAGS)

; multiboot header
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

; stack of 16 KiB
section bss
align 16
stack_bottom:
resb 16384
stack_top:

extern kernel_main

section .text
global _start
_start:
  ; setup stack
  mov esp, [stack_top]

  ; enter C kernel
  call kernel_main

  ; halt infinitely if kernel exits
  cli
  hlt
  jmp $