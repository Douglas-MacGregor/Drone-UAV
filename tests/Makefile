CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpigpio -lrt -lpthread

TARGET = unittest
OBJS = unittest_mySX1278.o mySX1278.o utils.o unity.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

unity.o: unity.h unity.c unity_internals.h
	$(CC) -c unity.c $(CFLAGS)

unittest_mySX1278.o: ../src/mySX1278.h unittest_mySX1278.c
	$(CC) -c unittest_mySX1278.c $(CFLAGS)

mySX1278.o: ../src/mySX1278.c ../src/mySX1278.h ../src/utils.h
	$(CC) -c ../src/mySX1278.c $(CFLAGS)

utils.o: ../src/utils.c ../src/utils.h
	$(CC) -c ../src/utils.c $(CFLAGS)

clean:
	rm -f *.o unittest
