#ifndef PLATFORM_H
#define PLATFORM_H

#define CLOCK_RATE    10000000L
#define TIMER_COUNT   (CLOCK_RATE / SYSTICKS_HZ)

#define SP804_0_BASE  0x101e2000
#define SP804_1_BASE  0x101e2020
#define SP804_2_BASE  0x101e3000
#define SP804_3_BASE  0x101e3020

#define UART0_BASE    0x101f1000

#endif