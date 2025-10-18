        .data
a:      .dword  9,8,0,1,0,5,3,1,2,0
tam:    .dword 10         # Vector size
cont:   .dword 0          # Nr of elements == 0

        .text
start:  addi t0,gp,a      # Pointer
        ld t1,tam(gp)     # Vector size
        add t2,zero,zero  # Counter of zeros
 
loop:
        ld t3, 0(t0)
        addi t1, t1, -1  # Decrease remaining element count
        addi t0, t0, 8  # Move vector pointer
        bne t3, x0, endloop
        addi t2, t2, 1

        endloop:
        bne t1, x0, loop  # Continue if there are still elements
    
        la t4, cont
        sd t2, 0(t4)
        ori x17,x0,10     # Exit system call
        ecall
