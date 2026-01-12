CC = gcc
CFLAGS = -g -Wall

all: dualshock_device.o sx1278_utils.o

dualshock_device.o: dualshock_device.c dualshock_device.h ../../utils.h ../controller_interface.h dualshock_utils.h
	$(CC) -c dualshock_device.c $(CFLAGS)

dualshock_utils.o: dualshock_utils.c dualshock_utils.h ../../utils.h ../controller_interface.h
	$(CC) -c dualshock_utils.c $(CFLAGS)

clean:
	rm -f *.o