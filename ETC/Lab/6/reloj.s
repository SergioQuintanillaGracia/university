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
                jal pasa_segundo # Increase one more second
                la $a0, reloj
                jal imprime_reloj

salir:          li $v0, 10              # Código de exit (10)
                syscall                 # Última instrucción ejecutada
                .end


inicializa_reloj:
				li $t0, 0x001F3F3F  	# 00000000 00011111 00111111 00111111

				li $t1, 0

				and $t1, $t0, $a1
				sw $t1, 0($a0)

				jr $ra

devuelve_reloj_en_s:
				lbu $v0, 0($a0)			# The initial value of $v0 is the seconds

				lbu $t0, 2($a0)			# Hours
				li $t1, 3600
				mult $t0, $t1

				# Detect overflow
				mfhi $t2
				bgtz $t2, salir

				mflo $t1
				addu $v0, $v0, $t1


				lbu $t0, 1($a0)			# Minutes
				li $t1, 60
				mult $t0, $t1

				# Detect overflow
				mfhi $t2
				bgtz $t2, salir

				mflo $t1
				addu $v0, $v0, $t1

				jr $ra

devuelve_reloj_en_s_sd:
                lbu $v0, 0($a0)         # The initial value of $v0 is the seconds

                # Hours
                lbu $t0, 2($a0)
                li $t1, 3600
                sll $t2, $t0, 11
                addu $v0, $v0, $t2
                sll $t2, $t0, 10
                addu $v0, $v0, $t2
                sll $t2, $t0, 9
                addu $v0, $v0, $t2
                sll $t2, $t0, 4
                addu $v0, $v0, $t2

                # Minutes
                lbu $t0, 1($a0)
                li $t1, 60
                sll $t2, $t0, 5
                addu $v0, $v0, $t2
                sll $t2, $t0, 4
                addu $v0, $v0, $t2
                sll $t2, $t0, 3
                addu $v0, $v0, $t2
                sll $t2, $t0, 2
                addu $v0, $v0, $t2

                jr $ra

devuelve_reloj_en_s_srd:
                lbu $v0, 0($a0)         # The initial value of $v0 is the seconds

                # Hours
                lbu $t0, 2($a0)
                li $t1, 3600
                sll $t2, $t0, 12
                addu $v0, $v0, $t2
                sll $t2, $t0, 9
                subu $v0, $v0, $t2
                sll $t2, $t0, 5
                addu $v0, $v0, $t2
                sll $t2, $t0, 4
                subu $v0, $v0, $t2

                # Minutes
                lbu $t0, 1($a0)
                li $t1, 60
                sll $t2, $t0, 6
                addu $v0, $v0, $t2
                sll $t2, $t0, 2
                subu $v0, $v0, $t2

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
                lbu $t0, 0($a0)         # $t0 = SS
                addiu $t0, $t0, 1
                li $t1, 60
                beq $t0, $t1, S60
                sb $t0, 0($a0)
                j fin_pasa_segundo

S60:
                sb $zero, 0($a0)        # Set seconds to 0
                lbu $t0, 1($a0)         # $t0 = MM
                addiu $t0, $t0, 1
                beq $t0, $t1, M60
                sb $t0, 1($a0)
                j fin_pasa_segundo

M60:
                sb $zero, 1($a0)        # Set minutes to 0
                j pasa_hora

fin_pasa_segundo:
                jr $ra
