CC = gcc
CFLAGS = -g -Wall

all: mySX1278.o utils.o

mySX1278.o: mySX1278.c mySX1278.h utils.h
	$(CC) -c mySX1278.c $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -c utils.c $(CFLAGS)

clean:
	rm -f *.o