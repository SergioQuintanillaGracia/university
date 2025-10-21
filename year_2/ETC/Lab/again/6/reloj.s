                ##########################################################
                # Segmento de datos
                ##########################################################

                .data 0x10000000
reloj:          .word 0                # HH:MM:SS (3 bytes de menor peso)

cad_asteriscos: .asciiz "\n  **************************************"
cad_horas:      .asciiz "\n   Horas: "
cad_minutos:    .asciiz " Minutos: "
cad_segundos:   .asciiz " Segundos: "
cad_reloj_en_s: .asciiz "\n   Reloj en segundos: "

                ##########################################################
                # Segmento de código
                ##########################################################

                .globl __start
                .text 0x00400000

__start:        
				la $a0, reloj
				li $a1, 0x00173b3b # Time is 23:59:59
				jal inicializa_reloj
				la $a0, reloj
				jal imprime_reloj
				la $a0, reloj
				jal pasa_segundo # Increase one second
				la $a0, reloj
				jal imprime_reloj
				la $a0, reloj
				jal pasa_segundo # Increase one more second
				la $a0, reloj
				jal imprime_reloj
             
salir:          li $v0, 10              # Código de exit (10)
                syscall                 # Última instrucción ejecutada
                .end
				
devuelve_reloj_en_s_srd:
				li $v0, 0  # Second counter
				lw $t1, 0($a0)  # Clock value
				
				li $t2, 0x0000003F  # Seconds mask
				and $t2, $t1, $t2
				add $v0, $v0, $t2
				
				li $t2, 0x00003F00  # Minutes mask
				and $t2, $t1, $t2
				srl $t2, $t2, 8
				
				# t3 = t2 * 60
				sll $t3, $t2, 6
				sll $t4, $t2, 2
				subu $t3, $t3, $t4
				
				add $v0, $v0, $t3
				
				li $t2, 0x001F0000  # Hours mask
				and $t2, $t1, $t2
				srl $t2, $t2, 16
				
				# t3 = t2 * 3600
				sll $t3, $t2, 12
				sll $t4, $t2, 9
				subu $t3, $t3, $t4
				sll $t4, $t2, 5
				addu $t3, $t3, $t4
				sll $t4, $t2, 4
				subu $t3, $t3, $t4
				
				add $v0, $v0, $t3
				
				jr $ra

devuelve_reloj_en_s_sd:
				li $v0, 0  # Second counter
				lw $t1, 0($a0)  # Clock value
				
				li $t2, 0x0000003F  # Seconds mask
				and $t2, $t1, $t2
				add $v0, $v0, $t2
				
				li $t2, 0x00003F00  # Minutes mask
				and $t2, $t1, $t2
				srl $t2, $t2, 8
				
				# t3 = t2 * 60
				sll $t3, $t2, 5
				sll $t4, $t2, 4
				addu $t3, $t3, $t4
				sll $t4, $t2, 3
				addu $t3, $t3, $t4
				sll $t4, $t2, 2
				addu $t3, $t3, $t4
				
				add $v0, $v0, $t3
				
				li $t2, 0x001F0000  # Hours mask
				and $t2, $t1, $t2
				srl $t2, $t2, 16
				
				# t3 = t2 * 3600
				sll $t3, $t2, 11
				sll $t4, $t2, 10
				addu $t3, $t3, $t4
				sll $t4, $t2, 9
				addu $t3, $t3, $t4
				sll $t4, $t2, 4
				addu $t3, $t3, $t4
				
				add $v0, $v0, $t3
				
				jr $ra

devuelve_reloj_en_s:
				li $v0, 0  # Second counter
				lw $t1, 0($a0)  # Clock value
				
				li $t2, 0x0000003F  # Seconds mask
				and $t2, $t1, $t2
				add $v0, $v0, $t2
				
				li $t2, 0x00003F00  # Minutes mask
				and $t2, $t1, $t2
				srl $t2, $t2, 8
				
				li $t3, 60
				mult $t2, $t3
				mflo $t2
				
				add $v0, $v0, $t2
				
				li $t2, 0x001F0000  # Hours mask
				and $t2, $t1, $t2
				srl $t2, $t2, 16
				
				li $t3, 3600
				mult $t2, $t3
				mflo $t2
				mfhi $t3
				or $t2, $t2, $t3
				
				add $v0, $v0, $t2
				
				jr $ra

inicializa_reloj:
				li $t0 0x001F3F3F  # Mask
				and $t1, $t0, $a1
				sw $t1, 0($a0)
				jr $ra

                ########################################################## 
                # Subrutina que imprime el valor del reloj
                # Entrada: $a0 con la dirección de la variable reloj
                ########################################################## 

imprime_reloj:  move $t0, $a0
                la $a0, cad_asteriscos  # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall

                la $a0, cad_horas       # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall

                lbu $a0, 2($t0)         # Lee el campo HH
                li $v0, 1               # Código de print_int
                syscall

                la $a0, cad_minutos     # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall

                lbu $a0, 1($t0)         # Lee el campo MM
                li $v0, 1               # Código de print_int
                syscall

                la $a0, cad_segundos    # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall

                lbu $a0, 0($t0)         # Lee el campo SS
                li $v0, 1               # Código de print_int
                syscall

                la $a0, cad_asteriscos  # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall
                jr $ra

                ########################################################## 
                # Subrutina que imprime los segundos calculados
                # Entrada: $a0 con los segundos a imprimir
                ########################################################## 

imprime_s:      move $t0, $a0
                la $a0, cad_asteriscos  # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall


                la $a0, cad_reloj_en_s  # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall

                move $a0, $t0           # Valor entero a imprimir
                li $v0, 1               # Código de print_int
                syscall

                la $a0, cad_asteriscos  # Dirección de la cadena
                li $v0, 4               # Código de print_string
                syscall
                jr $ra
                
                ########################################################## 
                # Subrutina que incrementa el reloj en una hora
                # Entrada: $a0 con la dirección del reloj
                # Salida: reloj incrementado en memoria
                # Nota: 23:MM:SS -> 00:MM:SS
                ########################################################## 
                
pasa_hora:      lbu $t0, 2($a0)         # $t0 = HH
                addiu $t0, $t0, 1       # $t0 = HH++
                li $t1, 24
                beq $t0, $t1, H24       # Si HH==24 se pone HH a cero
                sb $t0, 2($a0)          # Escribe HH++
                j fin_pasa_hora
H24:            sb $zero, 2($a0)        # Escribe HH a 0
fin_pasa_hora:  jr $ra

pasa_segundo:
				lbu $t0, 0($a0)		# t0 = seconds
				addiu $t0, $t0, 1	# Add 1 second
				li $t1, 60
				beq $t0, $t1, pasa_minuto	# Seconds got to 60
				sb $t0, 0($a0)		# Store the increased second count
				jr $ra				# End
pasa_minuto:
				sb $zero, 0($a0)	# Set seconds to 0
				lbu $t0, 1($a0)		# t0 = minutes
				addiu $t0, $t0, 1	# Add 1 minute
				li $t1, 60
				beq $t0, $t1, pasa_hora_wrapper
				sb $t0, 1($a0)		# Store the increased minute count
				jr $ra				# End
pasa_hora_wrapper:
				sb $zero, 1($a0)	# Set minutes to 0
				j pasa_hora			# Add an hour