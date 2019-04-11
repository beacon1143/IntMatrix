# compiler
CC=gcc
# flags for compiler
CFLAGS=-c -Wall

all: im

im: main.o IntMatrix.o
	$(CC) main.o IntMatrix.o -o im

main.o: main.c
	$(CC) $(CFLAGS) main.c

IntMatrix.o: IntMatrix.c
	$(CC) $(CFLAGS) IntMatrix.c

clean:
	rm -rf *.o im