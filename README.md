# Drone-UAV

This repository contains code and resources for a Drone Unmanned Aerial Vehicle (UAV) project. The project aims to develop a versatile and efficient drone system for various applications.

## Considerations

**Controbution Guidelines:** If you plan to contribute to this project, please adhere to the established coding standards and guidelines. Ensure that your code is well-documented and tested. And **PLEASE MAKE THE CODE MODULAR** so it can be reused in other projects.

**Dependencies:** Ensure that all necessary libraries and dependencies are installed before building the project. Document any specific versions required.

**Hardware Compatibility:** The code is primarily designed for Raspberry Pi Zero 2 W. If using different hardware, verify compatibility and adjust configurations as needed.

**Testing:** Thoroughly test any changes or additions to the codebase. Include unit tests where applicable and document the testing procedures.

## Setup Instructions

Refer to [docs/Setup.md](docs/Setup.md) for detailed setup instructions, including enabling the SPI interface and installing necessary libraries.

## Using Git

Refer to [docs/GitUsage.md](docs/GitUsage.md) for guidelines on using Git effectively within this project, including branching strategies and commit message conventions.

## Building the Project

```bash

cmake -B build-host -DCMAKE_TOOLCHAIN_FILE=toolchains/host.cmake
cmake --build build-host
#Or
cmake --build build-host --target <target_name>
```

```bash
cmake -B build-rpi -DCMAKE_TOOLCHAIN_FILE=toolchains/raspberry_pi.cmake
cmake --build build-rpi
#Or
cmake --build build-rpi --target <target_name>
```

```bash
cmake -B build-stm32 -DCMAKE_TOOLCHAIN_FILE=toolchains/stm32.cmake
cmake --build build-stm32
#Or
cmake --build build-stm32 --target <target_name>
```
