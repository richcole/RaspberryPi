#ifndef TIMER_H

#include "types.h"

void timer_init();
void timer_next();
void timer_clear();

extern uint32 timer_intr;


#endif
