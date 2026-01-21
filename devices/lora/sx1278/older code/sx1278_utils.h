#ifndef SX1278_UTILS_H
#define SX1278_UTILS_H
#include <stdint.h>
#include "lora_interface.h"

typedef struct SX1278Data
{
    uint8_t address;
    uint8_t write;
    uint8_t *data_transmit;
    uint8_t transmit_length;
    uint8_t *data_receive;
    uint8_t receive_length;
} SX1278Data;

// Micro controller Hardware pin definitions
#define PIN_RESET 26
// sx1278 related defines
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

int init_sx1278();
void reset_sx1278(int spi_handle);
void close_sx1278(int spi_handle);
int read_sx1278(int spi_handle, SX1278Data *data);
int write_sx1278(int spi_handle, SX1278Data *data);
int poll_reg(int spi_handle, uint8_t reg_address, uint8_t mask, uint8_t expected_value, int max_attempts, int delay_us);

// SX1278 mode setting functions
int activate_lora(int spi_handle);
int deactivate_lora(int spi_handle);
int set_sleep_mode(int spi_handle);
int set_tx_mode(int spi_handle);
int set_rx_continuous_mode(int spi_handle);
int set_rx_single_mode(int spi_handle);
int set_stdby_mode(int spi_handle);

// RX, TX and SPI FIFO functions
int rx_set_base_address(int spi_handle, uint8_t address);
int tx_set_base_address(int spi_handle, uint8_t address);
int spi_set_fifo_addr_ptr(int spi_handle, uint8_t address);

void print_reg_values(int spi_handle);

#endif // SX1278_UTILS_H