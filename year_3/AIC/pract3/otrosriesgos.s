	.ireg 1,2,3,4,5,6
	.data
x:	.dword 10
y: 	.dword 11
z:	.dword 0
t: 	.dword 0
u:	.dword 0
w: 	.dword 0

	.text

dadd r1,r2,r3
dsub r4,r1,r5
dadd r7,r1,r6

ld r1,x($gp)
dadd r3,r1,r4

dadd r10,$gp,0
ld r1,x(r10)
ld r2,y(r10)

dadd r10,$gp,0
sd r1,z(r10)
sd r2,t(r10)

ld r1,x($gp)
sd r1,u($gp)
sd r1,w($gp)

ld r1,x($gp)
ld r2,y($gp)
slt r1,r2,r3
bnez r1,loop

slt r1,r2,r3
nop
bnez r1,loop

ld r1,x($gp)
bnez r1,loop

loop:
trap#0
