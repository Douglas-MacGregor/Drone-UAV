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

# MPU-6050 Self-Test — Step-by-Step (Condensed)

This document lists **only the required steps** to perform a hardware self-test on the MPU-6050.

---

## 1. Wake the Device

```text
PWR_MGMT_1 (0x6B) = 0x00
```

Wait ~100 ms.

---

## 2. Configure Required Ranges

Self-test requires fixed full-scale settings:

```text
GYRO_CONFIG  (0x1B) = 0x00   // ±250 dps
ACCEL_CONFIG (0x1C) = 0x10   // ±8 g
```

---

## 3. Measure Baseline Output (Self-Test OFF)

1. Ensure self-test bits are cleared
2. Read raw gyro and accel data
3. Average many samples (e.g. 200–1000)

```text
gyro_baseline[x,y,z]
accel_baseline[x,y,z]
```

---

## 4. Enable Self-Test

Enable all axis self-test bits:

```text
GYRO_CONFIG  (0x1B) = 0b11100000
ACCEL_CONFIG (0x1C) = 0b11100000
```

Wait ~50 ms for settling.

---

## 5. Measure Self-Test Output

1. Read raw gyro and accel data again
2. Average the same number of samples

```text
gyro_selftest[x,y,z]
accel_selftest[x,y,z]
```

---

## 6. Compute Self-Test Response (STR)

```text
STR_g = gyro_selftest − gyro_baseline
STR_a = accel_selftest − accel_baseline
```

---

## 7. Read Factory Trim Codes

Read self-test registers:

```text
SELF_TEST_X (0x0D)
SELF_TEST_Y (0x0E)
SELF_TEST_Z (0x0F)
SELF_TEST_A (0x10)
```

Extract XG_TEST, YG_TEST, ZG_TEST, XA_TEST, YA_TEST, ZA_TEST.

---

## 8. Compute Factory Trim (FT)

Use datasheet equations to compute:

```text
FT[Xg], FT[Yg], FT[Zg]
FT[Xa], FT[Ya], FT[Za]
```

---

## 9. Compare Against Factory Trim

For each axis:

```text
Deviation (%) = (STR − FT) / FT × 100
```

---

## 10. Pass / Fail Decision

- Gyro: deviation within ±14%
- Accel: deviation within ±14%

If any axis fails → **device fails self-test**.

---

## 11. Disable Self-Test

Restore normal operation:

```text
GYRO_CONFIG  (0x1B) = 0x00
ACCEL_CONFIG (0x1C) = 0x10
```
