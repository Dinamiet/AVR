#include "structure.h"
#include "timer.h"

#include <avr/io.h>
#include <stdbool.h>

static bool isRunning(Timer* timer);

static bool isRunning(Timer* timer) { return timer->Registers->ControlB & (1 << CS22 | 1 << CS21 | 1 << CS20); }

void Timer_Init(Timer* timer)
{
	*timer->PWR &= ~(1 << PRTIM2);
	timer->Registers->ControlA = 0;
	timer->Registers->ControlB &= ~(1 << WGM22);
	Timer_Stop(timer);
}

void Timer_Deinit(Timer* timer)
{
	Timer_Stop(timer);
	*timer->PWR |= (1 << PRTIM2);
}

void Timer_Configure(Timer* timer, TimerPrescaler prescaler)
{
	timer->Prescaler = prescaler;
	if (isRunning(timer))
		Timer_Start(timer); // Start timer to set updated prescaler values if already running
}

void Timer_EnableInterrupt(Timer* timer, TimerInterruptType type, TimerInterrupt handler)
{
	timer->RollOverHandler = handler;
	*timer->InterruptMask |= type;
}
