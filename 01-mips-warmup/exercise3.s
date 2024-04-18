.psx
.create "exercise3.bin", 0x80010000
.org 0x80010000

Main:
  move $t2, $zero
  li   $t0, 27
  li   $t1, 3

While:
  blt   $t0, $t1, EndWhile
  nop
  subu  $t0, $t0, $t1
  addiu $t2, $t2, 1
  b     While
  nop

EndWhile:

End:
.close
