#include <stdlib.h>
#include "./sx1278/sx1278_utils.h"
#include "../utils.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main()
{
    int spi_handle = init_spi();
    reset_sx1278(spi_handle);
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
    data.address = REG_FIFO_TX_BASE_ADDR;
    data.write = 0;
    uint8_t address;
    data.data_receive = &address;
    data.receive_length = 1;
    if (read_sx1278(spi_handle, &data) > 0)
    {
        fprintf(stdout, "FIFO TX Base Address: 0x%02X\n", address);
    }
    data.address = REG_FIFO_ADDR_PTR;
    data.write = 1;
    uint8_t fifo_ptr = address;
    data.data_transmit = &fifo_ptr;
    data.transmit_length = 1;
    if (write_sx1278(spi_handle, &data) > 0)
    {
        fprintf(stdout, "FIFO Address Pointer set to TX Base Address\n");
    }
    while (1)
    {
        data.address = REG_FIFO_ADDR_PTR;
        data.write = 1;
        data.data_transmit = &fifo_ptr;
        data.transmit_length = 1;
        write_sx1278(spi_handle, &data);

        data.address = REG_FIFO;
        data.write = 1;
        char text[] = "Hello";
        uint8_t *bytes = (uint8_t *)text;
        data.data_transmit = bytes;
        data.transmit_length = 5;
        if (write_sx1278(spi_handle, &data) > 0)
        {
            fprintf(stdout, "Data written to FIFO\n");
        }
        data.address = REG_PAYLOAD_LENGTH;
        data.write = 1;
        uint8_t payload_length = 5;
        data.data_transmit = &payload_length;
        data.transmit_length = 1;
        if (write_sx1278(spi_handle, &data) > 0)
        {
            fprintf(stdout, "Payload length set to %d\n", payload_length);
        }
        fprintf(stdout, "Transmitting...\n");
        data.address = REG_IRQ_FLAGS;
        data.write = 1;
        uint8_t irq_flags = 0;
        uint8_t clear = 0xFF;
        data.data_transmit = &clear;
        data.transmit_length = 1;
        write_sx1278(spi_handle, &data);
        data.write = 0;
        data.data_receive = &irq_flags;
        data.receive_length = 1;
        set_tx_mode(spi_handle);
        read_sx1278(spi_handle, &data);
        while ((irq_flags & IRQ_FLAG_TX_DONE) == 0)
        {
            read_sx1278(spi_handle, &data);
        }
        set_stdby_mode(spi_handle);
        data.address = REG_IRQ_FLAGS;
        data.write = 1;
        clear = 0xFF;
        data.data_transmit = &clear;
        data.transmit_length = 1;
        write_sx1278(spi_handle, &data);
        fprintf(stdout, "Transmission complete\n");
        usleep(300000);
    }
    close_spi(spi_handle);
    return 0;
}