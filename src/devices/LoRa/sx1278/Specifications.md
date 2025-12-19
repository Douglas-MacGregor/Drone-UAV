# sx1278

The SX1278 is a Semtech LoRa transceiver module that operates in the 137 MHz to 1020 MHz frequency range. It is widely used in IoT applications for long-range, low-power wireless communication.

## Key Specifications

can be found at: [docs](../../../../docs/sx1276_77_78_79.pdf)

## Our Implementation

There is two levels of implementation for the SX1278 module in this project:

1. **Low-Level Driver**: This layer handles direct communication with the SX1278 hardware via SPI. It includes functions for reading and writing registers, configuring the module, and managing power states.

2. **High-Level Interface**: This layer provides a more user-friendly API for sending and receiving data using the SX1278 module. It abstracts away the low-level details and offers functions for initializing the module, setting up communication parameters, and handling data transmission and reception.

The lower level functions are located in the `sx1278_utils.c` and `sx1278_utils.h` files, while the higher level interface is implemented in the `sx1278_device.c` and `sx1278_device.h` files.
