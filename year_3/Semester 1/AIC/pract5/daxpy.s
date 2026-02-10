# z = a*x + y
# Tam. de los vectores: 8 palabras
    .data
# Vector x
x:  .double 1, 2, 3, 4, 5, 6, 7, 8
# Vector y
y:  .double 100, 100, 100, 100, 100, 100, 100, 100
# Vector z
z:  .space 64
fin_z:
# Valor a
a:  .double 2

    .text
main:
    addi t1, gp, x
    addi t2, gp, y
    addi t3, gp, z
    addi t4, gp, fin_z
    fld ft0, a(gp)
loop:
    fld ft1, 0(t1)            # Load X
    fld ft2, 0(t2)            # Load Y
    fmul.d ft3, ft1, ft0      # a*X
    fadd.d ft4, ft2, ft3      # a*X +Y
    fsd ft4, 0(t3)            # Store Z
    addi t1, t1, 8
    addi t2, t2, 8
    addi t3, t3, 8
    bne t4, t3, loop
exit:
    li a7, 10                 # syscall exit
    ecall

