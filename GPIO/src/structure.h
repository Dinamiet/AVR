#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <stdint.h>

typedef struct _GPIO_
{
	volatile uint8_t Pin;
	volatile uint8_t DDR;
	volatile uint8_t Port;
} GPIO;

#endif
