.PHONY: clean
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpigpio -lrt -lpthread
DEPENDENCIES = sx1278/mySX1278.o ../utils.o
DEPENDENCIES_HEADERS = sx1278/mySX1278.h ../utils.h

all: tx rx

tx: tx.o $(DEPENDENCIES)
	$(CC) -o tx tx.o $(DEPENDENCIES) $(LDFLAGS)

rx: rx.o $(DEPENDENCIES)
	$(CC) -o rx rx.o $(DEPENDENCIES) $(LDFLAGS)

tx.o: ${DEPENDENCIES_HEADERS} tx.c
	$(CC) -c tx.c $(CFLAGS)

rx.o: ${DEPENDENCIES_HEADERS} rx.c
	$(CC) -c rx.c $(CFLAGS)

clean:
	rm -f *.o tx rx
	$(MAKE) -C ./sx1278 clean