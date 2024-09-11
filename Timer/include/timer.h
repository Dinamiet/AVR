#ifndef _TIMER_H_
#define _TIMER_H_

/**
 * \file
 * Timer
 *
 * Basic timer implementation for system timekeeping
 *
 * TODO: Improved more complete implementation, this is a very basic implementation
 */

#include "stdint.h"

typedef enum _TimerPrescaler_
{
	TIMER_DIV_1 = 1,
	TIMER_DIV_8,
	TIMER_DIV_32,
	TIMER_DIV_64,
	TIMER_DIV_128,
	TIMER_DIV_256,
	TIMER_DIV_1024,
} TimerPrescaler;

typedef enum _TimerInterruptType_
{
	TIMER_INTERRUPT_ROLLOVER = 0x01,
	/** TODO: Implement other timer interrupts */
} TimerInterruptType;

typedef enum _TimerInstance_
{
	TIMER0,
	/** TODO: Implement other timers */
} TimerInstance;

typedef void (*TimerInterrupt)(void);

typedef struct _Timer_ Timer;

Timer*   Timer_GetInstance(TimerInstance instance);
void     Timer_Init(Timer* timer);
void     Timer_Deinit(Timer* timer);
void     Timer_Configure(Timer* timer, TimerPrescaler prescaler);
void Timer_EnableInterrupt(Timer* timer, TimerInterruptType type, TimerInterrupt handler);
void     Timer_Start(Timer* timer);
void     Timer_Stop(Timer* timer);
uint16_t Timer_GetValue(Timer* timer);

#endif
