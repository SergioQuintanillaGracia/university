#-------------------------------------------------#
#                                                 #
#         LAB 12: POLLING SYNCHRONISATION         #
#                                                 #
#-------------------------------------------------#

# ACTIVITY 3:  Complete the following functions:
#    char getchar()       - read a character from keyboard 
#    void putchar(char c) - print character on the console

# Data segment

	.data		

#-------------------------------------------------#

# Code segment
	.text
    	.globl __start	

__start:			
	li $a0, 'P'		# 
	jal putchar		# putchar('P')
	li $a0, '1'		# 
	jal putchar		# putchar('1')
	li $a0, '2'		# 
	jal putchar		# putchar('2')
	li $a0, 13		# CR character ('\n')
	jal putchar		# putchar('\n') (i.e., new line)
	

loop:
	li $t0, 0x1b      	# ASCII code of ESC key (0x1b = 27)

	jal getchar		     # $v0 = getchar()
	move $a0, $v0		 #
	beq $a0, $t0, finish # Detect ESC key
	jal putchar		     # putchar($a0)
	# jal putcode
	li $a0, 13
	jal putchar
	b loop
finish:	
	li $v0, 10
	syscall			     # exit
	
	
getchar:        # $v0 = getchar()
    li $t2, 0xffff0000
	
	wait:   # Wait for bit R == 1
		lw $t1, 0($t2)
		andi $t1, $t1, 1
		beqz $t1, wait
		
	lw $v0, 4($t2)
	jr $ra      # return from getchar


putchar:        # putchar($a0)
    ### TO BE COMPLETED
	li $t2, 0xffff0008
	
	wait2:
		lw $t1, 0($t2)
		andi $t1, $t1, 1
		beqz $t1, wait2
	
	sw $a0, 4($t2)	
	jr $ra      # return from putchar


putcode:
	li $v0, 1
	syscall
	jr $ra