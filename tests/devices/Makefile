CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpigpio -lrt -lpthread


unittest_sx1278 = unittest_sx1278
unittest_mpu6050 = unittest_mpu6050
mpu6050_dir = ../../src/devices/IMU/MPU-6050
sx1278_dir = ../../src/devices/LoRa/sx1278
unittest_sx1278 = unittest_sx1278
unittest_mpu6050 = unittest_mpu6050
mpu6050_dir = ../../src/devices/IMU/MPU-6050
sx1278_dir = ../../src/devices/LoRa/sx1278

unittest_mpu6050_OBJS = \
	unity.o \
	unittest_mpu6050.o \
	$(mpu6050_dir)/mpu6050_utils.o \
	$ ../../src/devices/utils.o \
	${mpu6050_dir}/mpu6050_device.o

unittest_sx1278_OBJS = \
	unity.o \
	unittest_sx1278.o \
	$(sx1278_dir)/sx1278_utils.o \
	$ ../../src/devices/utils.o \
	${sx1278_dir}/sx1278_device.o

TARGET = $(unittest_sx1278) $(unittest_mpu6050)
TARGET = $(unittest_sx1278) $(unittest_mpu6050)

all: $(TARGET)

$(unittest_sx1278): $(unittest_sx1278_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(unittest_mpu6050): $(unittest_mpu6050_OBJS)
$(unittest_sx1278): $(unittest_sx1278_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(unittest_mpu6050): $(unittest_mpu6050_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

unity.o: ../unity.c ../unity.h ../unity_internals.h
	$(CC) -c ../unity.c $(CFLAGS)

$(unittest_sx1278).o: unittest_sx1278.c $(sx1278_dir)/sx1278_utils.h $(sx1278_dir)/sx1278_device.h
	$(CC) -c unittest_sx1278.c $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
	$(MAKE) -C $(sx1278_dir) clean
	$(MAKE) -C $(mpu6050_dir) clean
	$(MAKE) -C $(sx1278_dir) clean
	$(MAKE) -C $(mpu6050_dir) clean
