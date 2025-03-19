#ifndef __TIMER_H__
#define __TIMER_H__

#include <inttypes.h>

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

void init_timer();

#endif