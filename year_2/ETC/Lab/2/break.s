          .globl __start
          .text 0x00400000

__start:
          li $s0, 0
          li $s1, 0
loop:
          li $v0,5
          syscall
          beq $v0, $0, finish
          addu $s0,$s0,$v0
          li $v0,1
          move $a0, $s0
          syscall
          li $v0,11
          li $a0, '\n'
          syscall
          b loop

finish:
          li $v0,10
          syscall