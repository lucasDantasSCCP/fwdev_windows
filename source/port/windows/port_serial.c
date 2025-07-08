#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <tchar.h>

#include "hal.h"
#include "app.h"

static HANDLE hCom = INVALID_HANDLE_VALUE;
static DCB dcb;
static COMMTIMEOUTS timeouts;

static void port_serial_init(void)
{
    hCom = INVALID_HANDLE_VALUE;
    memset(&dcb, 0, sizeof(DCB));
    memset(&timeouts, 0, sizeof(COMMTIMEOUTS));
}

static void port_serial_configure(uint16_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits)
{
    
    if (hCom == INVALID_HANDLE_VALUE)
    {
        printf("Porta serial não está aberta.\n");
        return;
    }

    dcb.BaudRate = baud_rate;      // Taxa de transmissão
    dcb.ByteSize = byte_size;      // Tamanho do byte
    dcb.Parity   = parity;         // Paridade
    dcb.StopBits = stop_bits;      // Bits de parada

    if (!SetCommState(hCom, &dcb))
    {
        printf("Erro em SetCommState. Código do erro: %lu\n", GetLastError());
        return;
    }

    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hCom, &timeouts))
    {
        printf("Erro em SetCommTimeouts. Código do erro: %lu\n", GetLastError());
        return;
    }

    printf("Porta serial configurada com sucesso.\n");
}

static bool port_serial_open(uint8_t *com_port)
{
    hCom = CreateFileA((char*) com_port,                
                    GENERIC_READ | GENERIC_WRITE, 
                    0,                       
                    NULL,                    
                    OPEN_EXISTING,           
                    0,                       
                    NULL);

    if (hCom == INVALID_HANDLE_VALUE)
    {
        printf("Erro ao abrir %s. Código do erro: %lu\n", com_port, GetLastError());
        return 1;
    }

    ZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(hCom, &dcb))
    {
        printf("Erro em GetCommState. Código do erro: %lu\n", GetLastError());
        CloseHandle(hCom);
        return 1;
    }

    printf("%s aberto com sucesso.\n", com_port);
    return 0;
}

static void port_serial_close(void)
{
    if (hCom != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hCom);
        hCom = INVALID_HANDLE_VALUE;
        printf("Porta serial fechada.\n");
    }
    else
    {
        printf("Nenhuma porta serial aberta.\n");
    }
}

static void port_serial_write(const char *data, size_t length)
{
    DWORD bytesWritten;
    if (!WriteFile(hCom, data, length, &bytesWritten, NULL))
    {
        printf("Erro ao escrever na porta. Código do erro: %lu\n", GetLastError());
    }
    else
    {
        printf("Escreveu %lu bytes na porta.\n", bytesWritten);
    }
}

static void port_serial_read(char *buffer, size_t bufferSize)
{
    DWORD bytesRead;
    if (!ReadFile(hCom, buffer, bufferSize, &bytesRead, NULL))
    {
        printf("Erro ao ler da porta. Código do erro: %lu\n", GetLastError());
    }
    else
    {
        printf("Leu %lu bytes: %.*s\n", bytesRead, (unsigned long)bytesRead, buffer);
    }
}

static void port_serial_multiple_COM()
{
}

static void port_serial_interrupt()
{
}

hal_serial_driver_t HAL_SERIAL_DRIVER = 
{
    .serial_init = port_serial_init,
    .serial_open = port_serial_open,
    .serial_close = port_serial_close,
    .serial_configure = port_serial_configure,
    .serial_multiple = port_serial_multiple_COM,
    .serial_interrupt = port_serial_interrupt,  
};