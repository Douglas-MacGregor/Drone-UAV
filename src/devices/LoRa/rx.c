#include <stdlib.h>
#include "sx1278/sx1278_utils.h"
#include "../utils.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    int spi_handle = init_sx1278();
    if (spi_handle < 0)
    {
        return -1;
    }

    if (activate_lora(spi_handle) > 0)
    {
        fprintf(stdout, "LoRa mode activated\n");
    }
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_FIFO_ADDR_PTR;
    data.write = 1;
    uint8_t address = 0x00;
    data.data_transmit = &address;
    data.transmit_length = 1;
    if (write_sx1278(spi_handle, &data) > 0)
    {
        fprintf(stdout, "FIFO Address Pointer set to 0x00\n");
    }
    data.address = REG_FIFO_RX_BASE_ADDR;
    if (read_sx1278(spi_handle, &data) > 0)
    {
        fprintf(stdout, "FIFO RX Base Address: 0x%02X\n", address);
    }
    fprintf(stdout, "Receiving...\n");
    data.address = REG_IRQ_FLAGS;
    data.write = 0;
    uint8_t irq_flags = 0;
    uint8_t clear = 0xFF;
    data.write = 1;
    data.data_transmit = &clear;
    data.transmit_length = 1;
    write_sx1278(spi_handle, &data);
    data.write = 0;
    data.data_receive = &irq_flags;
    data.receive_length = 1;
    set_rx_continuous_mode(spi_handle);
    read_sx1278(spi_handle, &data);
    while ((irq_flags & IRQ_FLAG_RX_DONE) == 0)
    {
        read_sx1278(spi_handle, &data);
        usleep(1000);
    }
    set_stdby_mode(spi_handle);
    fprintf(stdout, "Data received!\n");
    data.address = REG_RX_NB_BYTES;
    data.write = 0;
    uint8_t num_bytes;
    data.data_receive = &num_bytes;
    data.receive_length = 1;
    if (read_sx1278(spi_handle, &data) > 0)
    {
        fprintf(stdout, "Number of bytes received: %d\n", num_bytes);
    }
    data.address = REG_FIFO;
    data.write = 0;
    uint8_t *received_data = malloc(num_bytes);
    data.data_receive = received_data;
    data.receive_length = num_bytes;
    if (read_sx1278(spi_handle, &data) > 0)
    {
        fprintf(stdout, "Received Data: ");
        for (int i = 0; i < num_bytes; i++)
        {
            fprintf(stdout, "%c", received_data[i]);
        }
        fprintf(stdout, "\n");
    }
    free(received_data);
    close_sx1278(spi_handle);
    return 0;
}