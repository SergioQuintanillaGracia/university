                ######################################################
                # Data segment
                ######################################################

                .data 0x10000000
A:              .word 0,1,2,3,4,5,6,7   # Vector A
                .data 0x10001000
B:              .space 32               # Vector B (result)
                .data 0x1000A030
k:              .word 7                 # Scalar constant
dim:            .word 8                 # Vector dimension

                ######################################################
                # Code segment
                ######################################################

                .text 0x00400000
                .globl __start
                
__start:        la $a0, A               # $a0 points to A 
                la $a1, B               # $a1 points to B 
                la $a2, k               # $a2 points to k 
                la $a3, dim             # $a3 points to dim
                jal sax                 # Subroutine call
                
                # End program 
                li $v0, 10              # Exit code
                syscall                 # Call exit function
                
                ######################################################
                # Subroutine that computes Y <- k*X 
                # $a0 = Starting address of vector X
                # $a1 = Starting address of vector Y
                # $a2 = Address of scalar constant k
                # $a3 = Address of dimension       
                ######################################################

sax:            lw $a2, 0($a2)          # $a2 = constant k
                lw $a3, 0($a3)          # $a3 = dimension
loop:           lw $t0, 0($a0)          # Read X[i] into $t0
                mult $a2, $t0           # Compute k*X[i]
                mflo $t0                # $t0 <- k*X[i] (HI value is 0)
                sw $t0, 0($a1)          # Write Y[i] 
                addi $a0, $a0, 4        # Address of X[i+1]
                addi $a1, $a1, 4        # Address of Y[i+1]
                addi $a3, $a3, -1       # Decrement the number of elements
                bgtz $a3, loop          # Jump while elements remain
                jr $ra                  # Subroutine return
                
                .end


