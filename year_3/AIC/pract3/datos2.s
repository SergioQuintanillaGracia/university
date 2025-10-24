        # the result must be t3=30, t4=20 y t5=40
        
        .ireg 0,0,0,0,0,10,20 # t1=10, t2=20
        .data
a:      .dword 30
        .text
        ld t3,a(gp)
        addi t4,t3,-10
        add t5,t1,t3
end:    ori a7,x0,10
        ecall
