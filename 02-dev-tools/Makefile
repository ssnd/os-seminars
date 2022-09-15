## src: https://makefiletutorial.com/#fancy-rules

CC = gcc # Flag for implicit rules
CFLAGS = -g # Flag for implicit rules. Turn on debug info

# Implicit rule #1: blah is built via the C linker implicit rule
# Implicit rule #2: blah.o is built via the C compilation implicit rule, because blah.c exists
blah: blah.o

a: blah.c
	echo "blah.c is present"

blah.c:
	echo "int main() { return 0; }" > blah.c

clean:
	rm -f blah*