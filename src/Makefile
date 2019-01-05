#compiler variables
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

objects = mmio.o routines.o prog.o

prog: $(objects)
	$(CC) $(LDFLAGS) $(objects) -o $@

.PHONY: clean
clean:
	$(RM) $(objects)
