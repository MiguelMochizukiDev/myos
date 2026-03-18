/* timer.h */

#ifndef DRIVERS_TIMER_H
#define DRIVERS_TIMER_H

#include <stdint.h>

void timer_init(void);
uint32_t timer_get_tick(void);

#endif
