#include "structure.h"
#include "timer.h"

#include <avr/io.h>

void Timer_Start(Timer* timer) { timer->Registers->ControlB |= timer->Prescaler; }

void Timer_Stop(Timer* timer) { timer->Registers->ControlB &= ~(1 << CS22 | 1 << CS21 | 1 << CS20); }

uint16_t Timer_GetValue(Timer* timer) { return timer->Registers->Count; }
