#include "hal.h"

static hal_serial_driver_t *drv = &HAL_SERIAL_DRIVER;

void hal_serial_init(void)
{
    drv->serial_init();
}

bool hal_serial_open(uint8_t *com_port)
{
    drv->serial_open(com_port);
}

void hal_serial_close(void)
{
    drv->serial_close();
}

void hal_serial_configure(uint16_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits)
{
    drv->serial_configure(baud_rate, byte_size, parity, stop_bits);
}

void hal_serial_multiple_COM(void)
{
    drv->serial_multiple();
}

void hal_serial_interrupt(void)
{
    drv->serial_interrupt();
}

void hal_serial_write(const char *data, size_t length)
{
    drv->serial_write(data, length);
}

void hal_serial_read(char *buffer, size_t bufferSize)
{
    drv->serial_read(buffer, bufferSize);
}
