          .globl __start
          .data 0x10000000
M:        .space 4
Q:        .space 4
R:        .space 4
 
          .text 0x00400000
__start:  
		  # Get M
          li $a0,'M'
          la $a1, M
          jal InputV
		  
		  # Get Q
		  li $a0,'Q'
          la $a1, Q
          jal InputV
		  
		  # Multiply
		  la $a0, M
		  la $a1, Q
		  la $a2, R
		  jal MultV
		  
		  # Print result
		  li $a0, 'R'
		  la $a1, R
		  jal OutputV

          # Terminar el proceso
          li $v0,10
          syscall

InputV:   li $v0, 11
          syscall
          li $v0, 11
          li $a0, '='
          syscall
		  # Get a number from the user
          li $v0, 5
          syscall
		  # Store the inputted number
          sw $v0, 0($a1)
          jr $ra

Output:   # Output acepta parámetros directamente en registros: 
          #   void Output (char $a0, int $a1)
          # Hay que cambiarla por OutputV, que opera sobre variables en memoria:
          #   void OutputV (char $a0, int *$a1)

          li $v0, 11        # print_char
          syscall           # Imprime $a0
          li $v0, 11        # print_char
          li $a0, '='       # Carácter ‘=’ en $a0
          syscall           # Imprime ‘=’
          li $v0, 1         # print_int
          move $a0, $a1     # Entero a imprimir en $a0
          syscall           # Imprime $a0
          li $v0, 11        # print_char
          li $a0, 10        # Carácter ASCII ‘\n’ en $a0
          syscall           # Imprime ‘\n’
          jr $ra            # Retorno de Output

MultR:    # MultR acepta parámetros y retorna el resultado directamente en registros:
          #    int Mult (int $a0, int $a1) 
          # Hay que cambiarla por MultV, que opera sobre variables en memoria:
          #    void MultV (int *$a0, int *$a1, int *$a2)

          mult $a0, $a1     # Multiplica argumentos
          mflo $v0          # Resultado en $v0
          jr $ra            # Retorno de Mult

OutputV:
		  # Print char in a0
		  li $v0, 11
		  syscall
		  
		  # Print '='
		  li $v0, 11
		  li $a0, '='
		  syscall
		  
		  # Load the word pointed by a1 in a0
		  lw $a0, 0($a1)
		  # Print it
		  li $v0, 1
		  syscall
		  
		  jr $ra

MultV:
		  # Load operands
		  lw $t0, 0($a0)
		  lw $t1, 0($a1)
		  
		  # Multiply
		  mult $t0, $t1
		  mflo $t0
		  
		  # Store the result in the address in a2
		  sw $t0, 0($a2)
		  
		  jr $ra