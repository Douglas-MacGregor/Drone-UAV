#ifndef SX1278_DEVICE_H
#define SX1278_DEVICE_H
#include "lora_interface.h"

typedef struct
{
    LoRaInterface *vtable; // pointer to interface
    int spi_handle;        // SPI handle for communication
    int gpio_reset_pin;    // GPIO pin for reset
} sx1278_Device;

extern LoRaInterface sx1278_lora_interface;

int sx1278_init(void *self);
int sx1278_close(void *self);
int sx1278_send(void *self, const uint8_t *data, int len);
int sx1278_receive(void *self, uint8_t *buffer, int len);
int sx1278_set_syncword(void *self, uint8_t syncword);
int sx1278_set_frequency(void *self, uint32_t freq);
int sx1278_set_power(void *self, int level);
int sx1278_set_spreading_factor(void *self, int sf);
int sx1278_sleep(void *self);
int sx1278_standby(void *self);
int sx1278_reset(void *self);

sx1278_Device create_sx1278_device(int spi_handle, int gpio_reset_pin);
int destroy_sx1278_device(sx1278_Device *device);

typedef enum
{
    REG_FIFO = 0x00,
    REG_OPMODE = 0x01,
    REG_FIFO_ADDR_PTR = 0x0D, // SPI interface address pointer in FIFO data buffer
    REG_FIFO_TX_BASE_ADDR = 0x0E,
    REG_FIFO_RX_BASE_ADDR = 0x0F,
    REG_FIFO_RX_CURRENT_ADDR = 0x10, // Address of start of last packet received
    REG_IRQ_FLAGS_MASKS = 0x11,      // Interrupt mask register
    REG_IRQ_FLAGS = 0x12,            // Interrupt status register
    REG_RX_NB_BYTES = 0x13,          // Number of payload bytes received in last packet
    REG_MODEM_CONFIG_1 = 0x1D,
    REG_MODEM_CONFIG_2 = 0x1E,
    REG_PAYLOAD_LENGTH = 0x22,
    REG_SYNC_WORD = 0x39
    // Add more registers here...
} sx127x_reg_t;

// OP_MODE Register Bitfields
typedef enum
{
    OPMODE_DEFAULT = 0b10001001,
    OPMODE_LONG_RANGE = 0b10000000,
    OPMODE_SLEEP = 0b00000000,
    OPMODE_STDBY = 0b00000001,
    OPMODE_FSTX = 0b00000010,
    OPMODE_TX = 0b00000011,
    OPMODE_FSRX = 0b00000100,
    OPMODE_RX_CONTINUOUS = 0b00000101,
    OPMODE_RX_SINGLE = 0b00000110
} sx127x_opmode_bits_t;

typedef enum
{
    IRQ_MASK_CAD_DETECTED = 0b00000001,
    IRQ_MASK_FHSS_CHANGE_CHANNEL = 0b00000010,
    IRQ_MASK_CAD_DONE = 0b00000100,
    IRQ_MASK_TX_DONE = 0b00001000,
    IRQ_MASK_VALID_HEADER = 0b00010000,
    IRQ_MASK_PAYLOAD_CRC_ERROR = 0b00100000,
    IRQ_MASK_RX_DONE = 0b01000000,
    IRQ_MASK_RX_TIMEOUT = 0b10000000
} sx127x_IRQ_MASK_bits_t;

typedef enum
{
    IRQ_FLAG_CAD_DETECTED = 0b00000001,
    IRQ_FLAG_FHSS_CHANGE_CHANNEL = 0b00000010,
    IRQ_FLAG_CAD_DONE = 0b00000100,
    IRQ_FLAG_TX_DONE = 0b00001000,
    IRQ_FLAG_VALID_HEADER = 0b00010000,
    IRQ_FLAG_PAYLOAD_CRC_ERROR = 0b00100000,
    IRQ_FLAG_RX_DONE = 0b01000000,
    IRQ_FLAG_RX_TIMEOUT = 0b10000000
} sx127x_IRQ_FLAG_bits_t;

#endif // SX1278_DEVICE_H