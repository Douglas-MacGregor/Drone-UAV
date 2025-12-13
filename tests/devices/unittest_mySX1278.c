#include "../unity.h"
#include "../../src/devices/LoRa/sx1278/sx1278_utils.h"
#include "../../src/devices/LoRa/sx1278/sx1278_device.h"
#include "../../src/devices/utils.h"

#include <string.h>
#include <unistd.h>
#include <stdint.h>

int spi_handle;

void setUp(void)
{
    printf("\n----- Starting Test -----\n");
    print_reg_values(spi_handle);
}

void tearDown(void)
{
    print_reg_values(spi_handle);
    printf("----- Test Complete -----\n");
}

void test_read_write_sx1278(void)
{
    spi_handle = init_sx1278();
    reset_sx1278(spi_handle);
    deactivate_lora(spi_handle);
    reset_sx1278(spi_handle);
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = 0x04;
    data.write = 1;
    uint8_t value_to_write = 0x12;
    data.data_transmit = &value_to_write;
    data.transmit_length = 1;
    int write_result = write_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, write_result);
    data.address = 0x04;
    data.write = 0;
    uint8_t value_read = 0;
    data.data_receive = &value_read;
    data.receive_length = 1;
    int read_result = read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8(0x12, value_read);
    close_sx1278(spi_handle);
    return;
}

void test_lora_mode_sx1278(void)
{
    spi_handle = init_sx1278();
    reset_sx1278(spi_handle);
    int m = deactivate_lora(spi_handle);
    if (m < 0)
    {
        fprintf(stderr, "Failed to deactivate LoRa mode\n");
        fprintf(stderr, "m=%d\n", m);
    }
    reset_sx1278(spi_handle);
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t mode;
    data.data_receive = &mode;
    data.receive_length = 1;
    int read_result = read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT & ~(0b10000000)), mode);
    int activate_result = activate_lora(spi_handle);
    usleep(100);
    TEST_ASSERT_GREATER_OR_EQUAL(0, activate_result);
    data.address = REG_OPMODE;
    data.write = 0;
    data.data_receive = &mode;
    data.receive_length = 1;
    read_result = read_sx1278(spi_handle, &data);
    TEST_ASSERT_GREATER_OR_EQUAL(0, read_result);
    TEST_ASSERT_EQUAL_UINT8(OPMODE_DEFAULT & ~(0b111), mode);
    close_sx1278(spi_handle);
    return;
}

void test_set_modes_sx1278(void)
{
    spi_handle = init_sx1278();
    reset_sx1278(spi_handle);
    int m = deactivate_lora(spi_handle);
    if (m < 0)
    {
        fprintf(stderr, "Failed to deactivate LoRa mode\n");
        fprintf(stderr, "m=%d\n", m);
    }
    reset_sx1278(spi_handle);
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t mode;
    data.data_receive = &mode;
    data.receive_length = 1;
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8(OPMODE_DEFAULT & ~(0b10000000), mode);
    activate_lora(spi_handle);
    usleep(1000);
    mode = 0;
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8(OPMODE_DEFAULT & ~(0b111), mode);

    set_stdby_mode(spi_handle);
    usleep(1000);
    mode = 0;
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT & ~(0b111)) | OPMODE_STDBY, mode);

    set_sleep_mode(spi_handle);
    usleep(1000);
    mode = 0;
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT & ~(0b111)) | OPMODE_SLEEP, mode);

    set_stdby_mode(spi_handle);
    usleep(1000);
    mode = 0;
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT & ~(0b111)) | OPMODE_STDBY, mode);

    set_tx_mode(spi_handle);
    usleep(10);
    mode = 0;
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT & ~(0b111)) | OPMODE_TX, mode);

    set_rx_continuous_mode(spi_handle);
    usleep(10);
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT & ~(0b111)) | OPMODE_RX_CONTINUOUS, mode);

    set_rx_single_mode(spi_handle);
    usleep(10);
    mode = 0;
    read_sx1278(spi_handle, &data);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT & ~(0b111)) | OPMODE_RX_SINGLE, mode);
    close_sx1278(spi_handle);
    return;
}

void test_Lora_sx1278_device_creation_standby_sleep(void)
{
    spi_handle = init_spi();
    sx1278_Device device = create_sx1278_device(spi_handle);
    TEST_ASSERT_EQUAL_INT(spi_handle, device.spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t mode;
    data.data_receive = &mode;
    data.receive_length = 1;
    int read_result = read_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8(OPMODE_DEFAULT, mode);
    device.vtable->standby(&device);
    mode = 0;
    data.address = REG_OPMODE;
    data.write = 0;
    data.data_receive = &mode;
    data.receive_length = 1;
    read_result = read_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8(((OPMODE_DEFAULT & ~(0b111)) | OPMODE_STDBY), mode);
    device.vtable->sleep(&device);
    mode = 0;
    data.address = REG_OPMODE;
    data.write = 0;
    data.data_receive = &mode;
    data.receive_length = 1;
    read_result = read_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8(((OPMODE_DEFAULT & ~(0b111)) | OPMODE_SLEEP), mode);
    device.vtable->close(&device);
    close_spi(spi_handle);
    return;
}

void test_Lora_sx1278_device_syncword(void)
{
    spi_handle = init_spi();
    sx1278_Device device = create_sx1278_device(spi_handle);
    TEST_ASSERT_EQUAL_INT(spi_handle, device.spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t mode;
    data.data_receive = &mode;
    data.receive_length = 1;
    int read_result = read_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8(OPMODE_DEFAULT, mode);
    uint8_t syncword = 0x77;
    device.vtable->set_syncword(&device, syncword);
    uint8_t read_syncword = 0;
    data.address = REG_SYNC_WORD;
    data.write = 0;
    data.data_receive = &read_syncword;
    data.receive_length = 1;
    read_result = read_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8(syncword, read_syncword);
    device.vtable->close(&device);
    close_spi(spi_handle);
    return;
}

void test_lora_sx1278_device_reset(void)
{
    spi_handle = init_spi();
    sx1278_Device device = create_sx1278_device(spi_handle);
    TEST_ASSERT_EQUAL_INT(spi_handle, device.spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 1;
    uint8_t mode = OPMODE_DEFAULT | OPMODE_RX_CONTINUOUS;
    data.data_transmit = &mode;
    data.transmit_length = 1;
    int write_result = write_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, write_result);
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t read_mode = 0;
    data.data_receive = &read_mode;
    data.receive_length = 1;
    int read_result = read_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8((OPMODE_DEFAULT | OPMODE_RX_CONTINUOUS), read_mode);
    device.vtable->reset(&device);
    read_result = read_sx1278(device.spi_handle, &data);
    TEST_ASSERT_EQUAL_INT(2, read_result);
    TEST_ASSERT_EQUAL_UINT8(OPMODE_DEFAULT, read_mode);
    device.vtable->close(&device);
    close_spi(spi_handle);
    return;
};

void test_lora_sx1278_device_rx_tx(void)
{
    spi_handle = init_spi();
    sx1278_Device device = create_sx1278_device(spi_handle);
    TEST_ASSERT_EQUAL_INT(spi_handle, device.spi_handle);
    device.vtable->send(&device, (uint8_t *)"Hello", 5);
    uint8_t buffer[5];
    device.vtable->receive(&device, buffer, 5);
    TEST_ASSERT_EQUAL_MEMORY((uint8_t *)"Hello", buffer, 5);
    device.vtable->close(&device);
    close_spi(spi_handle);
    return;
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_read_write_sx1278);
    RUN_TEST(test_lora_mode_sx1278);
    RUN_TEST(test_set_modes_sx1278);
    RUN_TEST(test_Lora_sx1278_device_creation_standby_sleep);
    RUN_TEST(test_Lora_sx1278_device_syncword);
    RUN_TEST(test_lora_sx1278_device_reset);
    RUN_TEST(test_lora_sx1278_device_rx_tx);
    return UNITY_END();
}
