#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct hal_serial_driver_s
{
    void (*serial_init)(void);
    bool (*serial_open)(uint8_t *com_port);
    void (*serial_close)(void);
    void (*serial_configure)(uint16_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits);
    void (*serial_multiple)(void);
    void (*serial_interrupt)(void);
    void (*serial_write)(const char *data, size_t length);
    void (*serial_read)(char *buffer, size_t bufferSize);
} hal_serial_driver_t;

void hal_serial_init(void);
bool hal_serial_open(uint8_t *com_port);
void hal_serial_close(void);
void hal_serial_configure(uint16_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits);
void hal_serial_multiple_COM(void);
void hal_serial_interrupt(void);
void hal_serial_write(const char *data, size_t length);
void hal_serial_read(char *buffer, size_t bufferSize);

#ifdef __cplusplus
}
#endif

