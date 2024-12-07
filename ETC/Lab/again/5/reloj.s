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
				li $a1, 66765
				jal inicializa_reloj_en_s
				
				la $a0, reloj
				jal imprime_reloj
             
salir:          li $v0, 10              # Código de exit (10)
                syscall                 # Última instrucción ejecutada
                .end

inicializa_reloj_en_s:
				li $t0, 60
				
				div $a1, $t0
				mfhi $a3  # a3 contains seconds
				mflo $t1  # 1 contains minutes
				
				div $t1, $t0
				mfhi $a2  # a2 contains minutes
				mflo $a1  # a1 contains hours
				
				j inicializa_reloj_alt

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

inicializa_reloj_alt:
				li $t0, 0  # Used to build the value of the time variable
				
				li $t1, 0x001F0000  # Hours mask
				sll $a1, $a1, 16
				and $t1, $t1, $a1
				or $t0, $t0, $t1
				
				li $t1, 0x00003F00  # Minutes mask
				sll $a2, $a2, 8
				and $t1, $t1, $a2
				or $t0, $t0, $t1
				
				li $t1, 0x0000003F  # Seconds mask
				and $t1, $t1, $a3
				or $t0, $t0, $t1
				
				# Now, t0 contains the value of the time variable
				sw $t0, 0($a0)
				
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
