# don't forget this works only on computers that have the arm64 gcc and qemu installed

COMPILER = aarch64-linux-gnu-gcc
EXECUTOR = qemu-aarch64 -L /usr/aarch-linux-gnu
EXECUTABLE = mem-access
FLAGS = -g


build:
	$(COMPILER) -o $(EXECUTABLE) $(FLAGS) mem-access.c mem-access-function.S 

exec: 
	$(EXECUTOR) $(EXECUTABLE)

cleanup: 
	rm $(EXECUTABLE)

run: build exec cleanup