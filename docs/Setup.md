# Setup Instructions

## Table of Contents

- [Raspberry Pi Zero 2 W Setup Instructions](#raspberry-pi-zero-2-w-setup-instructions)
  - [Enabling SPI Interface](#enabling-spi-interface)
  - [Install Necessary Libraries](#install-necessary-libraries)
  - [Reduced Setup Instructions](#reduced-setup-instructions)

## Raspberry Pi Zero 2 W Setup Instructions

When mapping to raspberry pi zero 2 W using physical pin numbers

| Raspberry Pi Physical Pin | SX1278 Pin | Description          |
| ------------------------- | ---------- | -------------------- |
| 3.3V 1                    | VCC        | Power Supply         |
| GND 6                     | GND        | Ground               |
| 24                        | NSS        | Chip Select          |
| 23                        | SCK        | Serial Clock         |
| 19                        | MOSI       | Master Out Slave In  |
| 21                        | MISO       | Master In Slave Out  |
| 37                        | RST        | Reset (with pull-up) |
| 16                        | DIO0       | Interrupt Pin        |

---

| Raspberry Pi Physical Pin | I2C Device Pin | Description               |
| ------------------------- | -------------- | ------------------------- |
| 3.3V 1                    | VCC            | Power Supply              |
| GND 6                     | GND            | Ground                    |
| 3                         | SDA            | Data Line (with pull-up)  |
| 5                         | SCL            | Clock Line (with pull-up) |

### Enabling SPI Interface

#### 1. Enable SPI and I2C using raspi-config

1. Open a terminal (or SSH in).
2. Run:

```bash
   sudo raspi-config
```

3. Navigate to:

   Interface Options → SPI
   Interface Options → I2C

4. Select **Yes** to enable SPI.
5. Select **Yes** to enable I2C.
6. Exit and reboot:

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

Check for I2C devices:

```bash
     ls /dev/i2c-*
```

You should see:

    /dev/i2c-1 /dev/i2c-2

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
    #> Interface Options > SPI > Yes
    #> Interface Options > I2C > Yes
    sudo reboot
    sudo apt update
    sudo apt install pigpio
```
