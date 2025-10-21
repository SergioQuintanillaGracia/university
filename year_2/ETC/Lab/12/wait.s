#-------------------------------------------------#
#
#        LAB 12: POLLING SYNCHRONISATION
# 
#-------------------------------------------------#

# ACTIVITY 2:  Add the instruction that reads the keyboard 
#              data register on the appropriate location


#------- Data segment ---------#
	.data		
T1:	.asciiz "Waiting...\n"
T2:	.asciiz "\nEnd\n"

#------- Code segment ---------#
	.text
    	.globl __start	

__start:
	# Print message T1 on the console 
	li $v0, 4
	la $a0, T1
	syscall	
	
	# Load keyboard base address
	la $t0, 0xffff0000

wait:   # Wait for bit R == 1
	lw $t1, 0($t0)
	andi $t1, $t1, 1
	beqz $t1,wait

        ### TO BE COMPLETED: Read keyboard data register
	lw $t2, 4($t0)

	# Print message T2 on the console
	li $v0, 4
	la $a0, T2
	syscall

	# exit
	li $v0, 10
	syscall			

	.end