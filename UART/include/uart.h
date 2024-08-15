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

UART*  UART_GetInstance(UARTInstance instance);
void   UART_Init(UART* uart, void* rxBuff, size_t rxSize, void* txBuff, size_t txSize);
void   UART_Enable(UART* uart);
void   UART_Disable(UART* uart);
void   UART_SetBaud(UART* uart, int baud);
void   UART_SetFormat(UART* uart, size_t dataBits, size_t stopBits, UARTParity parity);
size_t UART_Write(UART* uart, void* data, size_t size);
size_t UART_Read(UART* uart, void* data, size_t size);
bool   UART_IsBusy(UART* uart);

#endif
