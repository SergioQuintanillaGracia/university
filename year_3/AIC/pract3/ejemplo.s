        # adds the components of vector y until it finds
        # a component equal to 0
        # stores the result in a

        # the result must be a=6

        .data
a:      .dword  0
y:      .dword  1,2,3,0,4,5,6,7,8
        .text
        add t1,x0,x0  # t1=0
        add t3,x0,x0  # t3=0
        addi t2,gp,y  # t2 traverses y
        nop
loop:   add t1,t3,t1 
        ld t3,0(t2)   # t3 is y[i]
        addi t2,t2,8
        nop
        bnez t3,loop  # if t3<>0
        sd t1,a(gp)
end:    ori a7,x0,10
        ecall
