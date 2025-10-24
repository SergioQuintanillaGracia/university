        # the result must be t3=30, t4=20 y t5=40
        
        .ireg t1=10,t2=20
        .text
        add t3,t1,t2
        addi t4,t3,-10
        add t5,t1,t3
end:    ori a7,x0,10
        ecall
