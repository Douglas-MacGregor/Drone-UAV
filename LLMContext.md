# LLM Context documentation

This document provides context for the codebase of the Drone-UAV project, which is written in C and utilizes various libraries and hardware components. The project is structured into several directories, each serving a specific purpose.

## Project Structure

```
Drone-UAV/
├── src/
│   ├── devices/
│   │   ├── IMU/
│   │   │   ├── imu_interface.h
│   │   │   └── MPU_6050/
│   │   ├── LoRa/
│   │   │   ├── lora_interface.h
│   │   │   ├── sx1278/
│   │   └── utils.c
│   │   └── utils.h
│   ├── droneV0.1/
│   ├── pilotV0.1/
│
├── tests/
│   ├── devices/
│   │   └── unittest_mySX1278.c
│   ├── unityfiles
│   └── test_script.sh
│
├── docs/
│   ├── GitUsage.md
│   └── Setup.md
├── README.md
```

## Things of Note

A few important aspects of the project is to make all of the code modular so that it can be reused in other projects. The code is primarily designed for the Raspberry Pi Zero 2 W, and it is essential to ensure compatibility if different hardware is used.

Throrough testing is emphasized, with unit tests included where applicable. The project also follows specific Git usage guidelines to maintain a clean and organized codebase.

## Dependencies

The project relies on several libraries, including `pigpio` for GPIO control on the Raspberry Pi. Ensure that all necessary libraries and dependencies are installed before building the project, and document any specific versions required.

## Additional Comments
