#ifndef SX1278_UTILS_H
#define SX1278_UTILS_H

int read_sx1278(int spi_handle, uint8_t reg_address, uint8_t *value, int len);
int write_sx1278(int spi_handle, uint8_t reg_address, const uint8_t *value, int len);
int set_bits_sx1278(int spi_handle, uint8_t reg_address, uint8_t mask);
int clear_bits_sx1278(int spi_handle, uint8_t reg_address, uint8_t mask);
int poll_register_sx1278(int spi_handle, uint8_t reg_address, uint8_t mask, uint8_t expected_value, int max_attempts, int delay_us);
int reset_sx1278(int gpio_reset_pin);

#endif // SX1278_UTILS_H