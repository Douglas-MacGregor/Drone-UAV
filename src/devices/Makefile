CC = gcc
CFLAGS = -g -Wall

all : utils.o

utils.o: utils.c utils.h
	$(CC) -c utils.c $(CFLAGS)

clean:
	rm -f *.o