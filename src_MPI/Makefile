#compiler variables
CC = mpicc
CFLAGS = -Wall -g
LDFLAGS = -lm

objects = mmio.o prog.o

prog: $(objects)
	$(CC) $(LDFLAGS) $(objects) -o $@

.PHONY: clean
clean:
	$(RM) $(objects)