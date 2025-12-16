CC = gcc
CFLAGS = -g -Wall

all: mpu6050_utils.o mpu6050_device.o

mpu6050_device.o: mpu6050_device.c mpu6050_device.h mpu6050_utils.h ../imu_interface.h ../../utils.h
	$(CC) -c mpu6050_device.c $(CFLAGS)

mpu6050_utils.o: mpu6050_utils.c mpu6050_utils.h ../utils.h
	$(CC) -c mpu6050_utils.c $(CFLAGS)

clean:
	rm -f *.o
