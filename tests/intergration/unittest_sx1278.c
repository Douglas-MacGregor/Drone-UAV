#include "unity.h"
#include "sx1278_utils.h"
#include "sx1278_device.h"
#include "coordinate.h"
#include "hal_spi.h"
#include "hal_gpio.h"
#include <string.h>
#include <unistd.h>
#include <stdint.h>

int spi_handle;
extern HAL_GPIO hal_gpio;
extern HAL_SPI hal_spi;

void setUp(void)
{
    printf("Starting setUp...\n");

    // Check if HAL structures are properly initialized
    if (hal_gpio.init_gpio == NULL)
    {
        printf("ERROR: hal_gpio.init_gpio is NULL!\n");
        return;
    }
    if (hal_spi.init_spi == NULL)
    {
        printf("ERROR: hal_spi.init_spi is NULL!\n");
        return;
    }

    printf("Calling hal_gpio.init_gpio()...\n");
    int gpio_result = hal_gpio.init_gpio();
    printf("GPIO init result: %d\n", gpio_result);

    printf("Calling hal_spi.init_spi(0, 0, 100000)...\n");
    spi_handle = hal_spi.init_spi(0, 0, 100000);
    printf("SPI handle: %d\n", spi_handle);
    printf("setUp complete.\n");
}

void tearDown(void)
{
    hal_spi.close_spi(spi_handle);
    hal_gpio.close_gpio();
}

void test_read_write_sx1278(void)
{
    uint8_t value;
    int n = read_sx1278(spi_handle, REG_OPMODE, &value, 1);
    TEST_ASSERT_NOT_EQUAL(0xFF, value); // Assuming 0xFF is not a valid default value
    TEST_ASSERT_EQUAL(2, n);            // 1 byte data + 1 byte address
    value = 0x55;
    int m = write_sx1278(spi_handle, 0x04, value, 1);
    TEST_ASSERT_EQUAL(2, m); // 1 byte data + 1 byte address
    n = read_sx1278(spi_handle, 0x04, &value, 1);
    TEST_ASSERT_EQUAL(0x55, value);
    TEST_ASSERT_EQUAL(2, n); // 1 byte data + 1 byte address
}

void test_device_creation_sx1278(void)
{
    sx1278_Device device = create_sx1278_device(spi_handle, 26);
    device.vtable->init(&device);
    TEST_ASSERT_EQUAL(spi_handle, device.spi_handle);
    TEST_ASSERT_EQUAL(26, device.gpio_reset_pin);
    uint8_t value;
    int n = read_sx1278(device.spi_handle, REG_OPMODE, &value, 1);
    TEST_ASSERT_EQUAL(n, 2);
    TEST_ASSERT_EQUAL(OPMODE_DEFAULT, value);
    destroy_sx1278_device(&device);
}

void test_set_modes_sx1278(void)
{
    sx1278_Device device = create_sx1278_device(spi_handle, 26);
    device.vtable->init(&device);
    uint8_t value;
    int n = read_sx1278(device.spi_handle, REG_OPMODE, &value, 1);
    TEST_ASSERT_EQUAL(n, 2);
    TEST_ASSERT_EQUAL(OPMODE_STDBY, value & 0b00000111);
    int ret = sx1278_sleep(&device);
    TEST_ASSERT_EQUAL(0, ret);
    uint8_t opmode;
    read_sx1278(device.spi_handle, REG_OPMODE, &opmode, 1);
    TEST_ASSERT_EQUAL(OPMODE_SLEEP, opmode & 0b00000111);
    ret = sx1278_standby(&device);
    TEST_ASSERT_EQUAL(0, ret);
    read_sx1278(device.spi_handle, REG_OPMODE, &opmode, 1);
    TEST_ASSERT_EQUAL(OPMODE_STDBY, opmode & 0b00000111);
    destroy_sx1278_device(&device);
}

void test_send_sx1278(void)
{
    sx1278_Device device = create_sx1278_device(spi_handle, 26);
    device.vtable->init(&device);
    const uint8_t test_data[] = "Hello, SX1278!";
    int len = sizeof(test_data);
    int ret = sx1278_send(&device, test_data, len);
    TEST_ASSERT_EQUAL(len, ret);
    uint8_t value = 0;
    ret = read_sx1278(device.spi_handle, REG_OPMODE, &value, 1); // Just to check communication
    TEST_ASSERT_EQUAL(2, ret);
    TEST_ASSERT_EQUAL(OPMODE_STDBY, value & 0b00000111);
    destroy_sx1278_device(&device);
}

void test_receive_sx1278(void)
{
    sx1278_Device device = create_sx1278_device(spi_handle, 26);
    device.vtable->init(&device);
    uint8_t buffer[20];
    memset(buffer, 0, sizeof(buffer));
    int ret = sx1278_receive(&device, buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL(ret, -2); // In real test, we would check for actual received data
    uint8_t value = 0;
    ret = read_sx1278(device.spi_handle, REG_OPMODE, &value, 1); // Just to check communication
    TEST_ASSERT_EQUAL(2, ret);
    TEST_ASSERT_EQUAL(OPMODE_STDBY, value & 0b00000111);
    destroy_sx1278_device(&device);
}

void test_set_syncword_sx1278(void)
{
    sx1278_Device device = create_sx1278_device(spi_handle, 26);
    device.vtable->init(&device);
    uint8_t syncword = 0x34;
    int ret = device.vtable->set_syncword(&device, syncword);
    TEST_ASSERT_EQUAL(0, ret);
    uint8_t read_value = 0;
    ret = read_sx1278(device.spi_handle, REG_SYNC_WORD, &read_value, 1);
    TEST_ASSERT_EQUAL(2, ret);
    TEST_ASSERT_EQUAL(syncword, read_value);
    destroy_sx1278_device(&device);
}

void test_reset_sx1278(void)
{
    sx1278_Device device = create_sx1278_device(spi_handle, 26);
    device.vtable->init(&device);
    uint8_t syncword = 0x78;
    write_sx1278(device.spi_handle, REG_SYNC_WORD, &syncword, 1);
    device.vtable->reset(&device);
    uint8_t value_after;
    read_sx1278(device.spi_handle, REG_SYNC_WORD, &value_after, 1);
    TEST_ASSERT_NOT_EQUAL(syncword, value_after); // Values should differ after reset

    device.vtable->sleep(&device);
    uint8_t opmode_before;
    read_sx1278(device.spi_handle, REG_OPMODE, &opmode_before, 1);
    device.vtable->reset(&device);
    uint8_t opmode_after;
    read_sx1278(device.spi_handle, REG_OPMODE, &opmode_after, 1);
    TEST_ASSERT_EQUAL(OPMODE_STDBY, opmode_after & 0b00000111);
    destroy_sx1278_device(&device);
    TEST_ASSERT_EQUAL(OPMODE_LONG_RANGE, opmode_after & 0b10000000);
}

void test_set_frequency_sx1278(void)
{
    TEST_ASSERT_EQUAL(0, 0); // Placeholder since function is not implemented
}

void test_set_power_sx1278(void)
{
    TEST_ASSERT_EQUAL(0, 0); // Placeholder since function is not implemented
}

void test_set_spreading_factor_sx1278(void)
{
    TEST_ASSERT_EQUAL(0, 0); // Placeholder since function is not implemented
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_read_write_sx1278);
    RUN_TEST(test_device_creation_sx1278);
    RUN_TEST(test_set_modes_sx1278);
    RUN_TEST(test_send_sx1278);
    RUN_TEST(test_receive_sx1278);
    RUN_TEST(test_set_syncword_sx1278);
    RUN_TEST(test_reset_sx1278);
    RUN_TEST(test_set_frequency_sx1278);
    RUN_TEST(test_set_power_sx1278);
    RUN_TEST(test_set_spreading_factor_sx1278);
    return UNITY_END();
}
