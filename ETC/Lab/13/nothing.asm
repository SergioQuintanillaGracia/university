### ESTRUCTURA DE COMPUTADORES (ETSINF)
###
### Lab 13: Interrupt synchronisation
###
### nothing.asm : template code

###################################################################
##                  USER PROGRAM DATA                            ##
###################################################################
	.data		
var2:	.word 0x77777777

###################################################################
##                  USER PROGRAM CODE                            ##
###################################################################
	.text
main:

	jr $ra	# Just one return instruction

	.end