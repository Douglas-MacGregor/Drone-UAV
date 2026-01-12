CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpigpio -lrt -lpthread -lm


unittest_sx1278 = unittest_sx1278
unittest_mpu6050 = unittest_mpu6050
unittest_dualshock = unittest_dualshock
mpu6050_dir = ../../src/devices/IMU/MPU-6050
sx1278_dir = ../../src/devices/LoRa/sx1278
dualshock_dir = ../../src/devices/Controllers/DualShock

unittest_dualshock_OBJS = \
	unity.o \
	unittest_dualshock.o \
	$(dualshock_dir)/dualshock_utils.o \
	../../src/devices/utils.o \
	$(dualshock_dir)/dualshock_device.o

unittest_mpu6050_OBJS = \
	unity.o \
	unittest_mpu6050.o \
	$(mpu6050_dir)/mpu6050_utils.o \
	../../src/devices/utils.o \
	$(mpu6050_dir)/mpu6050_device.o

unittest_sx1278_OBJS = \
	unity.o \
	unittest_sx1278.o \
	$(sx1278_dir)/sx1278_utils.o \
	../../src/devices/utils.o \
	$(sx1278_dir)/sx1278_device.o

TARGET = $(unittest_sx1278) $(unittest_mpu6050) $(unittest_dualshock)

all: $(TARGET)

$(unittest_sx1278): $(unittest_sx1278_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(unittest_mpu6050): $(unittest_mpu6050_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(unittest_dualshock): $(unittest_dualshock_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

unity.o: ../unity.c ../unity.h ../unity_internals.h
	$(CC) -c ../unity.c $(CFLAGS)

$(unittest_sx1278).o: unittest_sx1278.c $(sx1278_dir)/sx1278_utils.h $(sx1278_dir)/sx1278_device.h
	$(CC) -c unittest_sx1278.c $(CFLAGS)

$(unittest_mpu6050).o: unittest_mpu6050.c $(mpu6050_dir)/mpu6050_utils.h $(mpu6050_dir)/mpu6050_device.h
	$(CC) -c unittest_mpu6050.c $(CFLAGS)

$(unittest_dualshock).o: unittest_dualshock.c $(dualshock_dir)/dualshock_utils.h $(dualshock_dir)/dualshock_device.h
	$(CC) -c unittest_dualshock.c $(CFLAGS)



clean:
	rm -f *.o $(TARGET)
	$(MAKE) -C $(sx1278_dir) clean
	$(MAKE) -C $(mpu6050_dir) clean
	$(MAKE) -C $(sx1278_dir) clean
	$(MAKE) -C $(mpu6050_dir) clean
