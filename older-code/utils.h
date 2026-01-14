#ifndef DEVICES_UTILS_H
#define DEVICES_UTILS_H
#define SPI_CHANNEL 0
#define SPI_SPEED 100000
#define SPI_MODE 0

int init_spi();
void close_spi(int spi_handle);
int init_gpio();
void close_gpio();
int init_i2c(uint8_t I2C_BUS, uint8_t I2C_ADDRESS);
void close_i2c(int i2c_handle);

#endif // DEVICES_UTILS_H