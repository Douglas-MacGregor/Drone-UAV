# LoRa SX1278 Module Specification

## Table of Contents

- [Raspberry Pi Connection](#raspberry-pi-connection)
  - [Enabling SPI Interface](#enabling-spi-interface)
  - [Installing Necessary Libraries](#installing-necessary-libraries)
  - [Reduced Setup Instructions](#reduced-setup-instructions)

## Raspberry Pi Connection

When mapping to raspberry pi zero 2 W using physical pin numbers

| Raspberry Pi Physical Pin | SX1278 Pin | Description          |
| ------------------------- | ---------- | -------------------- |
| 3.3V 1                    | VCC        | Power Supply         |
| GND 6                     | GND        | Ground               |
| 24                        | NSS        | Chip Select          |
| 23                        | SCK        | Serial Clock         |
| 19                        | MOSI       | Master Out Slave In  |
| 21                        | MISO       | Master In Slave Out  |
| 26                        | RST        | Reset (with pull-up) |
| 16                        | DIO0       | Interrupt Pin        |

### Enabling SPI Interface

#### 1. Enable SPI using raspi-config

1. Open a terminal (or SSH in).
2. Run:

```bash
   sudo raspi-config
```

3. Navigate to:

   Interface Options → SPI

4. Select **Yes** to enable SPI.
5. Exit and reboot:

```bash
   sudo reboot
```

---

#### 2. Check if SPI is enabled

After reboot, check for SPI devices:

```bash
    ls /dev/spi*
```

You should see:

    /dev/spidev0.0  /dev/spidev0.1

---

### Install Necessary Libraries

```bash
sudo apt update
sudo apt install pigpio
```

### Reduced Setup Instructions

For a quicker setup, you can use the following commands:

```bash
    sudo raspi-config
    sudo reboot
    ls /dev/spi*
    sudo apt update
    sudo apt install pigpio
```
