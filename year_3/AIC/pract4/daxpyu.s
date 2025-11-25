# z = a*x + y
    .data
# vector size: 60 elements
# vector x
x:  .double 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    .double 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    .double 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    .double 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    .double 40, 41, 42, 43, 44, 45, 46, 47, 48, 49
    .double 50, 51, 52, 53, 54, 55, 56, 57, 58, 59

# vector y
y:  .double 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
    .double 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
    .double 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
    .double 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
    .double 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
    .double 100, 100, 100, 100, 100, 100, 100, 100, 100, 100

# vector z
# 60 elements are 480 bytes
z:  .space 480

# scalar a
a:  .double 1

# code
    .text

start:
    addi t0, gp, x       # t0 points to x
    addi t1, gp, y       # t1 points to y
    addi t2, gp, z       # t2 points to z
    fld f0, a(gp)        # f0 holds a
    addi t3, t1, 480     # 60 elements are 480 bytes

loop:
    fld f3, 0(t0)       # x[n] 1
    fld f7, 8(t0)       # x[n] 2
    fld f11, 16(t0)      # x[n] 3
    fmul.d f4, f3, f0   # x[n] 1 * a
    fmul.d f8, f7, f0   # x[n] 2 * a
    fmul.d f12, f11, f0  # x[n] 3 * a
    fld f1, 0(t1)       # y[n] 1
    fld f5, 8(t1)       # y[n] 2
    fld f9, 16(t1)       # y[n] 3
    fadd.d f2, f4, f1   # a * x[n] 1 + y[n] 1
    fadd.d f6, f8, f5   # a * x[n] 2 + y[n] 2
    fadd.d f10, f12, f9   # a * x[n] 3 + y[n] 3
    fsd f2, 0(t2)
    fsd f6, 8(t2)
    fsd f10, 16(t2)
    addi t0, t0, 24
    addi t1, t1, 24
    sub t4, t3, t1
    addi t2, t2, 24
    bnez t4, loop

    ori a7, zero, 10     # end
    ecall




