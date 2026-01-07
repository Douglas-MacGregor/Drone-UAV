# mpu-6050

The MPU-6050 is a popular 6-axis Inertial Measurement Unit (IMU) that combines a 3-axis gyroscope and a 3-axis accelerometer on a single chip. It is widely used in various applications, including drones, robotics, and motion tracking systems. It is able to communicate via the I2C protocol. And can also manage additional sensors like a magnetometer via an auxiliary I2C bus.

## Key Specifications

can be found at:

- [Overview docs](../../../../docs/MPU-6000-Datasheet1.pdf)
- [Register Map](../../../../docs/RM-MPU-6000A.pdf)

## Our Implementation

There is two levels of implementation for the mpu6050 module in this project:

1. **Low-Level Driver**: This layer handles direct communication with the mpu6050 hardware via IIC. It includes functions for reading and writing registers, configuring the module, and managing power states.

2. **High-Level Interface**: This layer provides a more user-friendly API for sending and receiving data to the mpu6050 module. It abstracts away the low-level details and offers functions for initializing the module, setting up communication parameters, and handling data transmission and reception.

The lower level functions are located in the `mpu6050_utils.c` and `mpu6050_utils.h` files, while the higher level interface is implemented in the `mpu6050_device.c` and `mpu6050_device.h` files.
