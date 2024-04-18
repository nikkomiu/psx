.psx
.create "image.bin", 0x80010000

.org 0x80010000

; ----------------
; IO Port
; ----------------
IO_BASE_ADDR equ 0x1F80    ; IO Ports Memory map base address

; ----------------
; GPU Registers
; ----------------
GP0 equ 0x1810             ; GP0 @ $1F801810: Rendering data & VRAM Access
GP1 equ 0x1814             ; GP1 @ $1F801814: Display Control & Environment setup

IMG_WIDTH      equ 640
IMG_HEIGHT     equ 480
IMG_SIZE_BYTES equ 921600   ; 640 x 480 x 3 bytes per pixel (our image is 24bpp)

Main:
  ; ---------------------------------------------------------------------------
  ; Set $a0 as the global parameter with the IO_BASE_ADDR to be used by subs
  ; ---------------------------------------------------------------------------
  lui $a0, IO_BASE_ADDR

  li $t1, 0x00000000 ; 00 - Reset GPU
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x03000000 ; 03 - Display Enable
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x08000037 ; 08 - Display mode (640x480, 24-bpp, NTSC)
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x06C60260 ; 06 - Horiz Display Range 0xXXXxxx (3168-608)
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x0707E018 ; 07 - Vert Display Range %yyyyyyyyyyYYYYYYYYYY (504-24)
  sw $t1, GP1($a0)   ; Write to GP1

  ; ---------------------------------------------------------------------------
  ; Send commands to GP0
  ; ---------------------------------------------------------------------------
  li $t1, 0xE1000400 ; E1 - Draw Mode Settings
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0xE3000000 ; E3 - Draw Area TopLeft - %YYYYYYYYYYXXXXXXXXXX
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0xE403BD3F ; E4 - Drawing Area BottomRight - %YYYYYYYYYYXXXXXXXXXX
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0xE5000000 ; E5 - Drawing Offset - %YYYYYYYYYYXXXXXXXXXX
  sw $t1, GP0($a0)   ; Write to GP0

  ; ---------------------------------------------------------------------------
  ; Clear the screen
  ; ---------------------------------------------------------------------------
  li $t1, 0x02444444
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00000000
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00EF013F
  sw $t1, GP0($a0)   ; Write to GP0

  ; ---------------------------------------------------------------------------
  ; Copy rect data from CPU to GPU
  ; ---------------------------------------------------------------------------
  li    $t1, 0xA0000000     ; A0 - Copy Rect CPU to VRAM
  sw    $t1, GP0($a0)       ; Write GP0 Command

  li    $t1, 0x00000000     ; Copy Area, TopLeft (x=0, y=0)
  sw    $t1, GP0($a0)       ; Write GP0 Command

  li    $t1, 0x01E003C0     ; Copy Area, 0xHHHHWWWW (Height=480, Width=640)
  sw    $t1, GP0($a0)       ; Write GP0 Command

  li    $t0, IMG_SIZE_BYTES ; Load the total num of bytes of images
  srl   $t0, 2              ; t0 >> 2 == t0 / 4
  la    $t2, Image          ; $t2 = image offset
LoopWords:
  lw    $t1, 0($t2)         ; t1 = word from image array
  addiu $t2, 4              ; t2 += 4
  sw    $t1, GP0($a0)       ; Write to GP0
  bnez  $t0, LoopWords      ; if ($t0 != 0) keep looping
  addiu $t0, $t0, -1        ; t0--

LoopForever:
  j LoopForever
  nop

Image:
  .incbin "logo.bin"         ; include a 640x480 (total of 921,600 bytes)

End:

.close
