		.data    
zero:           .double 0.0               # Comienzo de los datos de memoria
filas_a:        .dword 5
cols_a:         .dword 5 # filas_b
cols_b:         .dword 5
                # Matriz A
a:		.double 1,2,3,4,5
		.double 10,20,30,40,50
                .double 100,200,300,400,500
                .double 0.1,0.2,0.3,0.4,0.5
                .double 0.11,0.12,0.13,0.14,0.15
		# Matriz B
b:		.double 1,0,0,0,0
		.double 0,1,0,0,0
                .double 0,0,1,0,0
                .double 0,0,0,1,0
                .double 0,0,0,0,1
		# Matriz C 25x8
c:		.space 200 

		.text                   # Comienzo del fragmento de código

main:
		addi t1,gp,A
		addi t2,gp,B
		addi t3,gp,C
	
		ld t4,filas_a(gp)	# Filas A
		ld t5,cols_a(gp)	# Columnas A, Filas B
		ld t6,cols_b(gp)	# Columnas B

                add s1,x0,x0
bucle_i:
                add s2,x0,x0
bucle_j:
                fld f3,zero(gp)
	
                mul s3,s1,t5 # i*cols
                add s3,s3,s2 # i*cols+j                
                slli s3,s3,3
                add s3,s3,t3
                add s4,x0,x0 
bucle_k:		
                # a[i,k]
                mul s5,s1,t5 # i*cols
                add s5,s5,s4 # i*cols+k                
                slli s5,s5,3
                add s5,s5,t1
		fld f0,0(s5)
                # b[k,j]
                mul s5,s4,t6 # k*cols
                add s5,s5,s2 # i*cols+j                
                slli s5,s5,3
                add s5,s5,t2
		fld f1,0(s5)
                # c[i,j]
                fmul.d f2,f0,f1
                fadd.d f3,f3,f2
		fsd f3,0(s3)

                addi s4,s4,1
		bne s4,t5,bucle_k
end_k:

		addi s2,s2,1
		bne s2,t6,bucle_j
end_j:
		addi s1,s1,1
		bne s1,t4,bucle_i
end_i:
exit:
        li a7, 10               # syscall exit
    ecall
