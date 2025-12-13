#include <pigpio.h>
#include "utils.h"

int init_spi()
{
    if (gpioInitialise() < 0)
    {
        fprintf(stdout, "pigpio init failed\n");
        return -1;
    }

    int spi_handle = spiOpen(SPI_CHANNEL, SPI_SPEED, SPI_MODE);
    if (spi_handle < 0)
    {
        fprintf(stdout, "spi init fail\n");
        gpioTerminate();
        return -1;
    }
    return spi_handle;
}

void close_spi(int spi_handle)
{
    spiClose(spi_handle);
    gpioTerminate();
    return;
}