          .globl __start
          .data 0x10000000
askfor:   .asciiz "Write something: "
written:  .asciiz "You have written: "
len:   .asciiz "String length is: "
string:   .space 80             

          .text 0x00400000
__start:  
		  # Read input string
		  la $a0, askfor        
          la $a1, string        
          li $a2, 80            
          jal InputS     
		  
		  # Print written string
		  la $a0, written
		  la $a1, string
		  jal OutputS
		  
		  # Get string length
		  la $a0, string
		  jal StrLength
		  
		  # Print string length text
		  move $s0, $v0
		  li $v0, 4
		  la $a0, len
		  syscall
		  
		  # Print string length value
		  li $v0, 1
		  move $a0, $s0
		  syscall

		  # Exit
          li $v0,10
          syscall

StrLength:
		  li $v0, 0  # String length
		  
		  loop:
			addi $v0, $v0, 1
			addi $a0, $a0, 1
			lb $t1, 0($a0)
			bnez $t1, loop
		
		  addi $v0, $v0, -1
		  
		  jr $ra

InputS:   
		  # Print the string `askfor`
		  li $v0, 4
          syscall
		  
		  # Read a string of maximum length 80 and store it in the address `string`
          li $v0, 8
          move $a0, $a1
          move $a1, $a2
          syscall
		  
          jr $ra

OutputS:
		  # Print first string
		  li $v0, 4
		  syscall
		  
		  # Print second string
		  move $a0, $a1
		  li $v0, 4
		  syscall
		  
		  jr $ra