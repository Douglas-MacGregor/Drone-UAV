CC = gcc
CFLAGS = -g -Wall

all: sx1278_device.o sx1278_utils.o

sx1278_device.o: sx1278_device.c sx1278_device.h ../../utils.h ../lora_interface.h sx1278_utils.h
	$(CC) -c sx1278_device.c $(CFLAGS)

sx1278_utils.o: sx1278_utils.c sx1278_utils.h ../../utils.h ../lora_interface.h
	$(CC) -c sx1278_utils.c $(CFLAGS)

clean:
	rm -f *.o