.psx
.create "factorialsub.bin", 0x80010000
.org 0x80010000

Main:
  li  $a0, 6    ; int num = 6
  jal Factorial ; call Factorial
  nop

LoopForever:
  j   LoopForever
  nop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Subroutine to compute the factorial of a number       ;;
;; Argument: num ($a0)                                   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Factorial:
  li $t1, 1 ; int i = 1
  li $t3, 1 ; int temp = 1
  li $t4, 1 ; int sum = 1

OuterLoop:
  bgt $t1, $a0, EndOuterLoop ; "while" i <= num
  nop

  move $t2, $zero ; j = 0
  move $t4, $zero ; sum = 0

InnerLoop:
  bge  $t2, $t1, EndInnerLoop ; "while" j < i
  nop

  add  $t4, $t4, $t3 ; sum += temp
  addi $t2, 1        ; j++

  j    InnerLoop
  nop

EndInnerLoop:

  move $t3, $t4 ; temp = sum
  addi $t1, 1   ; i++

  j    OuterLoop
  nop

EndOuterLoop:
  move $v0, $t4 ; put result in $v0
  jr   $ra      ; return to the calling routine

End:
.close
