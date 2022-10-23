FLAGS := -O3 -S -masm=intel
FILE := main.c
OUTPUT := main

all: asm asm-x87

asm:
	gcc $(FILE) $(FLAGS) -o $(OUTPUT).s

asm-x87:
	gcc $(FILE) $(FLAGS) -o $(OUTPUT)-x87.s -mfpmath=387

