PRAC=3

CFLAGS		= -I. -O2 -fno-strength-reduce -fPIC -Wall -Wno-unused -Wno-unused-result -Wno-format -Wno-switch
LFLAGS		= -lm
CC		= gcc

TARGET=riscv-m
#OBJECTS=codop.o control.o datapath.o enlazador.o estado.o g_string.o instrucciones.o log.o main.o memoria.o presentacion-html.o presentacion-txt.o registros.o riscv.o riscv_fp.o riscv_int.o riscv_syscall.o simbolos.o input.lex.o input.yacc.o riscv_int_alum.o riscv_alum.o
OBJECTS=codop.o control.o datapath.o enlazador.o estado.o g_string.o instrucciones.o log.o main.o memoria.o presentacion-html.o presentacion-txt.o registros.o riscv.o riscv_fp.o riscv_int.o riscv_syscall.o simbolos.o input.lex.o input.yacc.o


all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " [LINK] Building exec file '${TARGET}'"
	@$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS)

clean:
	@rm -vf $(OBJECTS) *.output

distclean: clean
	@rm -vf $(TARGET)

%.o: %.c
	@echo " [GCC] Building object file '$@' using $<"
	@$(CC) -c $(CFLAGS) -o $@ -D PRAC=$(PRAC) $<

%.s: %.c
	@echo " [GCC] Building asm file '$@' using $<"
	@$(CC) -S $(CFLAGS) -mtune=intel -o $@ -D PRAC=$(PRAC) $<

%.o: %.s
	@echo " [GCC] Building object file '$@' using $<"
	@$(CC) -c $(CFLAGS) -o $@ -D PRAC=$(PRAC) $<

%.lex.c: %.lex.l
	@echo " [FLEX] Building scanner file '$@' using $<"
	@lex -t $*.lex.l > $*.lex.c

input.lex.o: input.yacc.h

%.yacc.h: %.yacc.y
	@echo " [BISON] Building parser file '$@' using $<"
	@bison -o $*.yacc.c -d $*.yacc.y

%.yacc.c: %.yacc.y
	@echo " [BISON] Building parser file '$@' using $<"
	@bison -v -o $*.yacc.c $*.yacc.y 

