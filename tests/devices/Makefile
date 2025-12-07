CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpigpio -lrt -lpthread

TARGET = unittest_mySX1278

MY_SX1278_DIR = ../../src/devices/LoRa/sx1278

OBJS = \
	unity.o \
	unittest_mySX1278.o \
	$(MY_SX1278_DIR)/mySX1278.o \
	$(MY_SX1278_DIR)/utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

unity.o: unity.c unity.h unity_internals.h
	$(CC) -c unity.c $(CFLAGS)

unittest_mySX1278.o: unittest_mySX1278.c $(MY_SX1278_DIR)/mySX1278.h
	$(CC) -c unittest_mySX1278.c $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
	$(MAKE) -C $(MY_SX1278_DIR) clean
