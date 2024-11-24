#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include "timer.h"

#include <stdint.h>

typedef struct _TimerMemory_
{
	uint8_t ControlA;
	uint8_t ControlB;
	uint8_t Count;
	uint8_t CompareA;
	uint8_t CompareB;
	uint8_t Reserved;
	uint8_t Asynch;
} TimerMemory;

typedef struct _Timer_
{
	volatile TimerMemory* const Registers;
	volatile uint8_t* const     PWR;
	volatile uint8_t* const     InterruptMask;
	TimerInterruptHandler       RollOver;
	uint8_t                     Prescaler;
} Timer;

#endif
