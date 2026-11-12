bits 16
org 0x7c00

boot:
  cli   ; disable interrupts

  ; clear screen
  mov ah, 0x00  ; set video mode
  mov al, 0x03  ; standard text mode
  int 0x10

  ; print boot message "Bear Metal!"
  mov ah, 0x0e
  mov al, 'B'
  int 0x10

  mov ah, 0x0e
  mov al, 'e'
  int 0x10

  mov ah, 0x0e
  mov al, 'a'
  int 0x10

  mov ah, 0x0e
  mov al, 'r'
  int 0x10

  mov ah, 0x0e
  mov al, ' '
  int 0x10

  mov ah, 0x0e
  mov al, 'M'
  int 0x10

  mov ah, 0x0e
  mov al, 'e'
  int 0x10

  mov ah, 0x0e
  mov al, 't'
  int 0x10

  mov ah, 0x0e
  mov al, 'a'
  int 0x10

  mov ah, 0x0e
  mov al, 'l'
  int 0x10

  mov ah, 0x0e
  mov al, '!'
  int 0x10

  halt  ; stop execution

; define magic word at last 2 bytes
times 510 - ($ - $$) db 0
dw 0xAA55