.psx
.create "exercise2.bin", 0x80010000
.org 0x80010000

Main:
  li   $t0, 1
  move $t2, $zero

Loop:
  add  $t1, $t0, $t1
  addi $t0, $t0, 1
  ble  $t0, 10,  Loop
  nop

End:
.close
