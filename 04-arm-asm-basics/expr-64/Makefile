# don't forget this works only on computers that have the arm64 gcc and qemu installed

COMPILER = aarch64-linux-gnu-gcc
EXECUTOR = qemu-aarch64 -L /usr/aarch-linux-gnu
EXECUTABLE = expr-64
FLAGS = -g

build:
	$(COMPILER) -o $(EXECUTABLE) $(FLAGS) expr-64.S expr-64-test.c

exec: 
	$(EXECUTOR) $(EXECUTABLE)

cleanup: 
	rm $(EXECUTABLE)

run: build exec cleanup