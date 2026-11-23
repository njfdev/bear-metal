%define ALIGN 1
%define MEMINFO 2
%define VIDEO_MODE 4
%define FLAGS ALIGN + MEMINFO + VIDEO_MODE
%define MAGIC 0x1badb002 ; magic multiboot flag for GRUB
%define CHECKSUM -(MAGIC + FLAGS)

; multiboot header
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM
resb 5*4; skip to byte 32 of multiboot header for video setup
dd 0 ; linear graphics mode
dd 640 ; width in px
dd 480 ; height in px
dd 32 ; bit depth of each pixel

; stack of 16 KiB
section .bss
align 16
stack_bottom:
resb 16384
stack_top:

extern kernel_main

section .text
global _start
_start:
  ; setup stack
  mov esp, stack_top

  ; push multiboot info struct and magic onto stack for kernel_main
  push ebx
  push eax

  ; enter C kernel
  call kernel_main

  ; halt infinitely if kernel exits
  cli
  hlt
  jmp $

global interrupt_handler_entry
extern interrupt_handler
interrupt_handler_entry:
  call interrupt_handler
  iret