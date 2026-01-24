#include "sx1278_device.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "hal_gpio.h"
#include "hal_spi.h"

extern HAL_GPIO hal_gpio;
extern HAL_SPI hal_spi;

int main()
{
    hal_gpio.init_gpio();
    int spi_hanlde = hal_spi.init_spi(0, 0, 100000);
    uint8_t data[128];
    sx1278_Device device = create_sx1278_device(spi_hanlde, 26);
    device.vtable->init(&device);
    for (int i = 0; i < 128; i++)
    {
        int n = device.vtable->receive(&device, &data[0], 128);
        if (n > 0)
        {
            printf("Received %d bytes: ", n);
            for (int j = 0; j < n; j++)
            {
                printf("%c ", data[j]);
            }
            printf("\n");
        }
        usleep(100000); // Sleep for 100ms
    }
    return 0;
}