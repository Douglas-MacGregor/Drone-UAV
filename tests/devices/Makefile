CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpigpio -lrt -lpthread


SM1278_unittest = SM1278_unittest
MY_SX1278_DIR = ../../src/devices/LoRa/sx1278

SM1278_unittest_OBJS = \
	unity.o \
	unittest_mySX1278.o \
	$(MY_SX1278_DIR)/sx1278_utils.o \
	$ ../../src/devices/utils.o \
	${MY_SX1278_DIR}/sx1278_device.o

TARGET = $(SM1278_unittest)

all: $(TARGET)

$(SM1278_unittest): $(SM1278_unittest_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

unity.o: ../unity.c ../unity.h ../unity_internals.h
	$(CC) -c ../unity.c $(CFLAGS)

$(SM1278_unittest).o: unittest_mySX1278.c $(MY_SX1278_DIR)/sx1278_utils.h $(MY_SX1278_DIR)/sx1278_device.h
	$(CC) -c unittest_mySX1278.c $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
	$(MAKE) -C $(MY_SX1278_DIR) clean
