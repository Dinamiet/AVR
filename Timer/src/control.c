#include "structure.h"
#include "timer.h"

#include <assert.h>
#include <avr/io.h>
#include <stddef.h>

void Timer_Start(Timer* timer)
{
	assert(timer != NULL);
	assert(timer->Prescaler > 0);

	timer->Registers->ControlB |= timer->Prescaler;
}

void Timer_Stop(Timer* timer)
{
	assert(timer != NULL);

	timer->Registers->ControlB &= ~(1 << CS22 | 1 << CS21 | 1 << CS20);
}

uint16_t Timer_GetValue(const Timer* timer)
{
	assert(timer != NULL);

	return timer->Registers->Count;
}
