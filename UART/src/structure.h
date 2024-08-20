#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include "fifobuffer.h"

#include <stdint.h>

typedef struct _UARTMemory_
{
	uint8_t  StatusA;
	uint8_t  StatusB;
	uint8_t  StatusC;
	uint8_t  Reserved;
	uint16_t BaudRate;
	uint8_t  Data;
} UARTMemory;

typedef struct _UART_
{
	volatile UARTMemory* const Registers;
	volatile uint8_t* const    PWR;
	FifoBuffer                 RxBuffer;
	FifoBuffer                 TxBuffer;
} UART;

#endif
