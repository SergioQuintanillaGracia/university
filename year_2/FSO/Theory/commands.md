### Compile input.c and produce an executable named output
gcc -o output input.c

### Compile input.c into an object file (input.o) (not an executable)
gcc -c input.c

### List symbols in object file (shows functions and variable names)
nm input.o

### Disassembles input.o and shows the corresponding assembly code
objdump -d input.o