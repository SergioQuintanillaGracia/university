        .data
        # Vector x
x:		.double 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
        .double 10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0
        .double 20.0,21.0,22.0,23.0,24.0,25.0,26.0,27.0,28.0,29.0
        .double 30.0,31.0,32.0,33.0,34.0,35.0,36.0,37.0,38.0,39.0
        .double 40.0,41.0,42.0,43.0,44.0,45.0,46.0,47.0,48.0,49.0
        .double 50.0,51.0,52.0,53.0,54.0,55.0,56.0,57.0,58.0,59.0
        .double 60.0,61.0,62.0,63.0
        # Vector y
y:      .double 100, 100, 100, 100, 100, 100, 100, 100
        .double 100, 100, 100, 100, 100, 100, 100, 100
        .double 100, 100, 100, 100, 100, 100, 100, 100
        .double 100, 100, 100, 100, 100, 100, 100, 100
        .double 100, 100, 100, 100, 100, 100, 100, 100
        .double 100, 100, 100, 100, 100, 100, 100, 100
        .double 100, 100, 100, 100, 100, 100, 100, 100
        .double 100, 100, 100, 100, 100, 100, 100, 100
        # Vector z, 64*8 bytes/elem
z:      .space 512
fin_z:
a:      .double 2

	.text
main:
	addi t1, gp, x
	addi t2, gp, y
	addi t3, gp, z
	addi t4, gp, fin_z
	fld ft0, a(gp)
loop:
	fld ft1, 0(t1)			# Load X
	fld ft2, 0(t2)			# Load Y
	fmul.d ft3, ft1, ft0	# a*X
	fadd.d ft4, ft2, ft3	# a*X +Y
	fsd ft4, 0(t3)			# Store Z
	addi t1, t1, 8
	addi t2, t2, 8
	addi t3, t3, 8
	bne t4, t3, loop
exit:
	li a7, 10               # syscall exit
    ecall
