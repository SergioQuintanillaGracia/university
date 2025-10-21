        .globl __start
        .text 0x00400000

__start:
        li $s0, 0  # Sum
        li $s1, 0  # Number count
loop:
        # Print number count
        li $v0, 1
        move $a0, $s1
        addi $a0, 1
        syscall

        # Print '>'
        li $v0, 11
        li $a0, '>'
        syscall

        # Ask for a number
        li $v0, 5
        syscall

        # If the number is 0, jump to `finish`
        beqz $v0, finish

        # Add the number to the sum
        addu $s0, $s0, $v0

        # Increase number count
        addi $s1, 1

        b loop

finish:
        # Print sum
        li $v0, 11
        li $a0, '='
        syscall
        li $v0, 1
        move $a0, $s0
        syscall

        # Print '\n'
        li $v0, 11
        li $a0, '\n'
        syscall

        # Print number count
        li $v0, 11
        li $a0, 'n'
        syscall
        li $v0, 11
        li $a0, '='
        syscall
        li $v0, 1
        move $a0, $s1
        syscall

        # Print '\n'
        li $v0, 11
        li $a0, '\n'
        syscall

        # End the program
        li $v0, 10
        syscall