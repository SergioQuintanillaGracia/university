.globl __start
.text 0x00400000

__start:
    # Get first number to multiply
    li $a0, 'M'
    jal Input
    move $s0, $v0  # We can't store it in $a0 because we use it for other things
                   # before Mult is called

    # Get second number to multiply
    li $a0, 'Q'
    jal Input
    move $a1, $v0

    # Multiply the numbers
    move $a0, $s0  # We stored the first number in $s0, we have to move it to $a0
    jal Mult
    move $a1, $v0

    # Output the result
    li $a0, 'R'
    jal Output

    # Exit program
    li $v0, 10
    syscall

Mult:       
    li $v0, 0
    beqz $a1, MultRet

MultFor:
    add $v0, $v0, $a0
    addi $a1, $a1, -1
    bne $a1, $zero, MultFor

MultRet: 
    jr $ra

Input:
    # Print `$a0` character and '='
    li $v0, 11
    syscall
    li $a0, '='
    li $v0, 11
    syscall

    # Ask for a value
    li $v0, 5
    syscall

    jr $ra

Output:
    # Print `$a0` character and '='
    li $v0, 11
    syscall
    li $a0, '='
    li $v0, 11
    syscall

    # Print the result
    move $a0, $a1
    li $v0, 1
    syscall

    jr $ra