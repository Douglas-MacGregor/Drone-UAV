#include "../unity.h"
#include "../../src/devices/LoRa/sx1278/sx1278_utils.h"
#include "../../src/devices/LoRa/sx1278/sx1278_device.h"
#include <string.h>
#include <unistd.h>
int spi_handle;

void setUp(void)
{
    spi_handle = init_sx1278();
}

void tearDown(void)
{
    close_sx1278(spi_handle);
}

void test_read_write_sx1278(void)
{
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
    return;
}

void test_lora_mode_sx1278(void)
{

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
    return;
}

void test_set_modes_sx1278(void)
{
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
    return;
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_read_write_sx1278);
    RUN_TEST(test_lora_mode_sx1278);
    RUN_TEST(test_set_modes_sx1278);
    return UNITY_END();
}
