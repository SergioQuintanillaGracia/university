        .data
a:      .dword  9,8,0,1,0,5,3,1,2,0
tam:    .dword 10         # Vector size
cont:   .dword 0          # Nr of elements == 0

        .text
start:  addi t0,gp,a      # Pointer
        ld t1,tam(gp)     # Vector size
        add t2,zero,zero  # Counter of zeros
 
loop:
        ...
        
        ori x17,x0,10     # Exit system call
        ecall
