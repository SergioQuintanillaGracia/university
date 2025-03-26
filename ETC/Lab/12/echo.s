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
	b loop
finish:	
	li $v0, 10
	syscall			     # exit
	
	
	
getchar:        # $v0 = getchar()
    ### TO BE COMPLETED


	jr $ra      # return from getchar




putchar:        # putchar($a0)
    ### TO BE COMPLETED


	jr $ra      # return from putchar
