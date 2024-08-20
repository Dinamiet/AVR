#ifndef _UART_H_
#define _UART_H_

#include <stdbool.h>
#include <stddef.h>

typedef enum _UARTParity_
{
	UART_PARITY_NONE,
	UART_PARITY_ODD,
	UART_PARITY_EVEN,
} UARTParity;

typedef enum _UARTInstance_
{
	UART0,
} UARTInstance;

typedef struct _UART_ UART;

UART*  UART_GetInstance(const UARTInstance instance);
void   UART_Init(UART* uart, void* rxBuff, const size_t rxSize, void* txBuff, const size_t txSize);
void   UART_Enable(const UART* uart);
void   UART_Disable(const UART* uart);
void   UART_SetBaud(const UART* uart, const uint32_t baud);
void   UART_SetFormat(const UART* uart, const size_t dataBits, const UARTParity parity, const size_t stopBits);
size_t UART_Write(UART* uart, const void* data, const size_t size);
size_t UART_Read(UART* uart, void* data, const size_t size);
bool   UART_IsBusy(const UART* uart);

#endif
