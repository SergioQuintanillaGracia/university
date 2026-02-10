    # Inicializa los registros de c.f.
    .fpreg f1=1.0, f2=2.0

    .data
a:  .double 2.0

    .text
    fadd.d f4, f2, f1
    fmul.d f6, f4, f1
    fld f4, a(gp)
    fmul.d f6, f6, f4
final:
    li a7, 10               # syscall exit
    ecall

