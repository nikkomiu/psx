.psx
.create "graphics.bin", 0x80010000

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

Main:
  ; ---------------------------------------------------------------------------
  ; Set $a0 as the global parameter with the IO_BASE_ADDR to be used by subs
  ; ---------------------------------------------------------------------------
  lui $a0, IO_BASE_ADDR

  li $t1, 0x00000000 ; 00 - Reset GPU
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x03000000 ; 03 - Display Enable
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x08000001 ; 08 - Display mode (320x240, 15-bit, NTSC)
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x06C60260 ; 06 - Horiz Display Range 0xXXXxxx (3168-608)
  sw $t1, GP1($a0)   ; Write to GP1

  li $t1, 0x07042018 ; 07 - Vert Display Range %yyyyyyyyyyYYYYYYYYYY (264-24)
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
  ; Draw a flat-shaded triangle
  ; ---------------------------------------------------------------------------  
  li $t1, 0x20FF0000 ; Three-point polygon, opaque with color
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00100010 ; Vertex one
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00200010 ; Vertex two
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00100020 ; Vertex three
  sw $t1, GP0($a0)   ; Write to GP0

  ; ---------------------------------------------------------------------------
  ; Draw a flat-shaded quad
  ; ---------------------------------------------------------------------------  
  li $t1, 0x2800FF00 ; Three-point polygon, opaque with color
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00300030 ; Vertex one
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00400030 ; Vertex two
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00300040 ; Vertex three
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00400040 ; Vertex four
  sw $t1, GP0($a0)   ; Write to GP0

  ; ---------------------------------------------------------------------------
  ; Draw a gouraud-shaded triangle
  ; ---------------------------------------------------------------------------  
  li $t1, 0x3000FF00 ; Three-point polygon, opaque with color
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00500050 ; Vertex one
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00FF0000 ; Color two
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00400080 ; Vertex two
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x000000FF ; Color three
  sw $t1, GP0($a0)   ; Write to GP0

  li $t1, 0x00900070 ; Vertex three
  sw $t1, GP0($a0)   ; Write to GP0

  ; ---------------------------------------------------------------------------
  ; Initialize the stack pointer to 0x00103CF0
  ; ---------------------------------------------------------------------------
  la $sp, 0x00103CF0

  ; ---------------------------------------------------------------------------
  ; Invoke draw flat-shaded triangle subroutine
  ; ---------------------------------------------------------------------------
  addiu $sp, -(4 * 7)   ; Subtract from stack pointer as we are going to "push" 7
                        ; words/"params" into memory
  li    $t0, 0xFFFF00   ; Param: Color
  sw    $t0, 0($sp)     ; Push arg to $sp+0
  li    $t0, 200        ; Param: x1
  sw    $t0, 4($sp)     ; Push arg to $sp+4
  li    $t0, 40         ; Param: y1
  sw    $t0, 8($sp)     ; Push arg to $sp+8
  li    $t0, 288        ; Param: x2
  sw    $t0, 12($sp)    ; Push arg to $sp+12
  li    $t0, 56         ; Param: y2
  sw    $t0, 16($sp)    ; Push arg to $sp+16
  li    $t0, 224        ; Param: x3
  sw    $t0, 20($sp)    ; Push arg to $sp+20
  li    $t0, 200        ; Param: y3
  sw    $t0, 24($sp)    ; Push arg to $sp+24
  jal   DrawFlatTriangle
  nop

LoopForever:
  j LoopForever
  nop

; ---------------------------------------------------------------------------
; Subroutine to draw a flat-shaded triangle
;
; Args:
;   $a0    = IO_BASE_ADDR (IO ports at 0x1F80****
;   $sp+0  = Color (0xBBGGRR)
;   $sp+4  = x1
;   $sp+8  = y1
;   $sp+12 = x2
;   $sp+16 = y2
;   $sp+20 = x3
;   $sp+24 = y3
; ---------------------------------------------------------------------------
DrawFlatTriangle:
  lw    $t1, 0($sp)       ; Load color param from the stack
  lui   $t0, 0x2000       ; Command: 0x20 (flat triangle)
  or    $t1, $t0, $t1     ; Command | Color
  sw    $t1, GP0($a0)     ; Write to GP0 (Command+Color)

  lw    $t3, 8($sp)       ; Load y1 param from stack
  lw    $t2, 4($sp)       ; Load x1 param from stack
  sll   $t3, $t3, 16      ; y1 <<= 16
  andi  $t2, $t2, 0xFFFF  ; x1 &= 0xFFFF (mask x1 into only 2 bytes)
  or    $t1, $t2, $t3     ; x1 | y1
  sw    $t1, GP0($a0)     ; Write to GP0 (Vertex 1)

  lw    $t3, 16($sp)      ; Load y2 param from stack
  lw    $t2, 12($sp)      ; Load x2 param from stack
  sll   $t3, $t3, 16      ; y1 <<= 16
  andi  $t2, $t2, 0xFFFF  ; x1 &= 0xFFFF (mask x1 into only 2 bytes)
  or    $t1, $t2, $t3     ; x1 | y1
  sw    $t1, GP0($a0)     ; Write to GP0 (Vertex 1)

  lw    $t3, 24($sp)      ; Load y3 param from stack
  lw    $t2, 20($sp)      ; Load x3 param from stack
  sll   $t3, $t3, 16      ; y1 <<= 16
  andi  $t2, $t2, 0xFFFF  ; x1 &= 0xFFFF (mask x1 into only 2 bytes)
  or    $t1, $t2, $t3     ; x1 | y1
  sw    $t1, GP0($a0)     ; Write to GP0 (Vertex 1)

  jr    $ra               ; Return to caller (address stored in $ra)
  addiu $sp, $sp, (4 * 7) ; Move stack pointer up 7 words to shirnk

End:

.close
