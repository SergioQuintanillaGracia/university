#-------------------------------------------------#
#
#  LAB SESSION 15: SYSTEM CALLS (II)
# 
#-------------------------------------------------#

### THIS PROGRAM TESTS MIMOSv3.handler


# Identifiers of system functions

		print_char  = 11
		read_char   = 12
		get_version = 90
		get_time    = 91
		wait_time   = 92

# Data segment

		.data
# Various message strings
msg_OS:			.asciiz "MIMOS v."
msg_seconds:	.asciiz " seconds\n"
msg_waitchar:	.asciiz "Waiting for a character..."
msg_charread:	.asciiz "\nChar read = "

# A buffer for printf_integer. DO NOT MODIFY!
buffer_int: 	.ascii "          "

#-------------------------------------------------#

# Code segment ("text")

	.text
    	.globl main	

main:


# Display OS name and version
	la $a0,msg_OS
	jal print_string
	li $v0,get_version
	syscall
	move $a0,$v0
	jal printf_integer
	jal print_NL


# Test program for all functions (except wait_time)
# NOTE 1: This program never ends. You need to stop it.
# NOTE 2: That is why we don't care about overwriting $ra using jal from here
# NOTE 3: If you want a main that terminates (with jr $ra), you must save $ra 
#         before making any "jal" and restore it just before returning.

	li $s0,0
bucle:
	# Say the time
	li $v0,get_time
	syscall
	move $a0,$v0
	jal printf_integer
	la $a0,msg_seconds
	jal print_string

	# Wait for a character
	la $a0,msg_waitchar
	jal print_string
	li $v0,read_char
	syscall
	move $s1,$a0

	# Say which character was typed
	la $a0,msg_charread
	jal print_string
	move $a0,$s1
	li $v1,print_char
	syscall
	jal print_NL
	b bucle




#-------------------------------------------------


# Auxiliary functions

print_string: # $a0: puntero a string acabado en \0
	move $t0,$a0
	lb $a0,0($t0)
	beq $a0,$zero,$L4
$L3:
	li $v0,print_char 
	syscall
	addiu $t0,$t0,1
	lb $a0,0($t0)
	bne $a0,$zero,$L3
$L4:
	jr $ra

#-------------------------------------------------

print_NL:	# sense paràmetres: escriu NL
	li $a0,'\n'
	li $v0,print_char 
	syscall
	jr $ra

#-------------------------------------------------

printf_integer: # $a0: valor entero
                         move $t0,$a0		# dividendo inicial
	       li $t1,0          	 	# cuenta de cifras
	       li $t2,10         		# divisor

$L1:	# bucle de cambio de base
	divu $t0,$t2		# división entre 10
	mfhi $t3          		# tomo el resto
	addiu $t3,$t3,'0' 		# calculo código ascii
	sb $t3,buffer_int($t1)	# guardo en buffer
	addi $t1,$t1,1		# avanzo puntero
	mflo $t0			# nou dividendo
	bne $t0,$zero,$L1

$L2:	# bucle de escritura
	addiu $t1,$t1,-1		# retrocedo en buffer
	lb $a0,buffer_int($t1)	# tomo carácter
	li $v0,print_char		# escribo carácter
	syscall			# llamada
	bne $t1,$zero,$L2	
	li $v0,print_char 
	jr $ra

#-------------------------------------------------

mucho_calculo:
	li $t0,0x0004FFFF
$L5:	addiu $t0,$t0,-1
	bne $t0,$zero,$L5
	jr $ra