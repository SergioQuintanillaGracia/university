    .data                   # Comienzo de los datos de memoria
a:  .double 10.5
b:  .double 2
c:  .double 20

x:  .space 8
y:  .space 8

    .text                   # Comienzo del fragmento de código
inicio:
    fld ft0, a(gp)          # Carga a
    fld ft1, b(gp)          # Carga b
    fld ft2, c(gp)          # Carga c
    fadd.d ft4, ft0, ft1    # t1 = a + b
    fmul.d ft5, ft2, ft4    # t2 = c * t1
    fsd ft4, x(gp)          # Guarda t1
    fsd ft5, y(gp)          # Guarda t2
final:
    li a7, 10               # syscall exit
    ecall
