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
    int n = hal_gpio.init_gpio();
    if (n < 0)
    {
        fprintf(stderr, "Failed to initialize GPIO\n");
        return -1;
    }
    usleep(100000); // Sleep for 100ms to allow GPIO to stabilize
    int spi_hanlde = hal_spi.init_spi(0, 0, 100000);
    char data[] = "Hello, SX1278!";
    sx1278_Device device = create_sx1278_device(spi_hanlde, 26);
    device.vtable->init(&device);
    int n = device.vtable->send(&device, (const uint8_t *)data, sizeof(data));
    if (n > 0)
    {
        printf("Sent %d bytes: %s\n", n, data);
    }
    else
    {
        printf("Failed to send data\n");
    }
    usleep(100000); // Sleep for 100ms
    device.vtable->close(&device);
    destroy_sx1278_device(&device);
    hal_spi.close_spi(spi_hanlde);
    hal_gpio.close_gpio();

    return 0;
}