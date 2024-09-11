#include "structure.h"
#include "timer.h"

#include <avr/interrupt.h>
#include <stddef.h>

static Timer timer0 = {.Registers = (TimerMemory*)&TCCR2A, .PWR = &PRR, .InterruptMask = &TIMSK2};

Timer* Timer_GetInstance(const TimerInstance instance)
{
	switch (instance)
	{
		case TIMER0:
			return &timer0;

		default:
			return NULL;
	}
}

ISR(TIMER2_OVF_vect)
{
	if (timer0.RollOverHandler)
		timer0.RollOverHandler();
}
