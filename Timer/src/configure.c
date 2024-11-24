#include "structure.h"
#include "timer.h"

#include <assert.h>
#include <avr/io.h>
#include <stdbool.h>

static bool isRunning(const Timer* timer);

static bool isRunning(const Timer* timer)
{
	assert(timer != NULL);

	return timer->Registers->ControlB & (1 << CS22 | 1 << CS21 | 1 << CS20);
}

void Timer_Init(Timer* timer)
{
	assert(timer != NULL);

	*timer->PWR &= ~(1 << PRTIM2);
	timer->Registers->ControlA = 0;
	timer->Registers->ControlB &= ~(1 << WGM22);
	Timer_Stop(timer);
}

void Timer_Deinit(Timer* timer)
{
	assert(timer != NULL);

	Timer_Stop(timer);
	*timer->PWR |= (1 << PRTIM2);
}

void Timer_Configure(Timer* timer, const TimerPrescaler prescaler)
{
	assert(timer != NULL);
	assert(prescaler > 0 && prescaler <= TIMER_DIV_1024);

	timer->Prescaler = prescaler;
	if (isRunning(timer))
		Timer_Start(timer); // Start timer to set updated prescaler values if already running
}

void Timer_EnableInterrupt(Timer* timer, const TimerInterruptType type, const TimerInterruptHandler isr_handler)
{
	assert(timer != NULL);
	assert(isr_handler != NULL);

	timer->RollOver = isr_handler;
	*timer->InterruptMask |= type;
}
