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

/**
 * Timer prescaler divider setup
 */
typedef enum _TimerPrescaler_
{
	TIMER_DIV_1 = 1, /** Timer running at system clock */
	TIMER_DIV_8,     /** Timer running at 1/8 system clock */
	TIMER_DIV_32,    /** Timer running at 1/32 system clock */
	TIMER_DIV_64,    /** Timer running at 1/64 system clock */
	TIMER_DIV_128,   /** Timer running at 1/128 system clock */
	TIMER_DIV_256,   /** Timer running at 1/256 system clock */
	TIMER_DIV_1024,  /** Timer running at 1/1024 system clock */
} TimerPrescaler;

/**
 * Different timer interrupts
 *
 * TODO: Implement other timer interrupts
 */
typedef enum _TimerInterruptType_
{
	TIMER_INTERRUPT_ROLLOVER = 0x01, /** Rollover or Overflow interrupt */
} TimerInterruptType;

/**
 * Available hardware Timer instances
 *
 * TODO: Implement other timers
 */
typedef enum _TimerInstance_
{
	TIMER0,
} TimerInstance;

/**
 * Interrupt handler for timer interrupts
 */
typedef void (*TimerInterruptHandler)(void);

/**
 * Timer
 */
typedef struct _Timer_ Timer;

/**
 * Retreives the provided hardware Timer instance
 * \param instance The Timer instance to return
 * \return Reference to Timer instance
 */
Timer* Timer_GetInstance(const TimerInstance instance);

/**
 * Initialize and power-on Timer hardware
 * The Timer will be initialized in an off/disabled state
 * \param timer Timer to initialize
 */
void Timer_Init(Timer* timer);

/**
 * Deinitialize and power-off Timer hardware
 * The Timer will be stopped before deinitialization
 * \param timer Timer to deinitialize
 */
void Timer_Deinit(Timer* timer);

/**
 * Configures timer parameters
 * \param timer The timer to configure
 * \param prescaler The prescale value the timer should use
 */
void Timer_Configure(Timer* timer, const TimerPrescaler prescaler);

/**
 * Enable a certain timer interrupt
 * \param timer The timer which interrupt should be enabled
 * \param type The type of interrupt to enable
 * \param isr_handler The function that will be called when the interrupt occurs
 */
void Timer_EnableInterrupt(Timer* timer, const TimerInterruptType type, const TimerInterruptHandler isr_handler);

/**
 * Start timer
 * \param timer The timer to start
 */
void Timer_Start(Timer* timer);

/**
 * Stop timer
 * \param timer The timer to stop
 */
void Timer_Stop(Timer* timer);

/**
 * Reads the current counter value of the timer
 * \param timer Timer's current value to retrieve
 * \return The current timer value
 */
uint16_t Timer_GetValue(const Timer* timer);

#endif
