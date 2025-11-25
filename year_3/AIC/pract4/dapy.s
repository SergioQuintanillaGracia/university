# z = a + y
    .data
# vector y
# vector size: 60 elements
y:  .double  0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
    .double 10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0
    .double 20.0,21.0,22.0,23.0,24.0,25.0,26.0,27.0,28.0,29.0
    .double 30.0,31.0,32.0,33.0,34.0,35.0,36.0,37.0,38.0,39.0
    .double 40.0,41.0,42.0,43.0,44.0,45.0,46.0,47.0,48.0,49.0
    .double 50.0,51.0,52.0,53.0,54.0,55.0,56.0,57.0,58.0,59.0

# vector z
# 60 elements are 480 bytes
z:  .space 480

# scalar a
a:  .double 1.0

    .text

start:
    addi t1, gp, y       # t1 points to y
    addi t2, gp, z       # t2 points to z
    fld f0, a(gp)        # f0 holds a
    addi t3, t1, 480     # 60 elements are 480 bytes
loop:
    fld f1, 0(t1)
    fadd.d f2, f0, f1
    fsd f2, 0(t2)
    addi t1, t1, 8
    sub t4, t3, t1
    addi t2, t2, 8
    bnez t4, loop

    ori a7, zero, 10     # end
    ecall
