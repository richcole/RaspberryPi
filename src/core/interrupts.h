#ifndef INTERUPTS_H
#define INTERUPTS_H

#define VIC_BASE    0x10140000
#define IRQ_STATUS  REG32(VIC_BASE + 0x00)
#define FIQ_STATUS  REG32(VIC_BASE + 0x04)
#define INT_SELECT  REG32(VIC_BASE + 0x0c)
#define INT_ENABLE  REG32(VIC_BASE + 0x10)
#define INT_DISABLE REG32(VIC_BASE + 0x14)
#define INT_SOFT    REG32(VIC_BASE + 0x18)
#define INT_ACK     REG32(VIC_BASE + 0x30)

#define INTERRUPT_WDOG      0
#define INTERRUPT_SWI       1
#define INTERRUPT_DEBUG_RX  2
#define INTERRUPT_DEBUG_TX  3
#define INTERRUPT_TIMER12   4
#define INTERRUPT_TIMER34   5
#define INTERRUPT_GPIO0     6
#define INTERRUPT_GPIO1     7
#define INTERRUPT_GPIO2     8
#define INTERRUPT_GPIO3     9
#define INTERRUPT_RTC       10
#define INTERRUPT_SSP       11
#define INTERRUPT_UART0     12
#define INTERRUPT_UART1     13
#define INTERRUPT_UART2     14
#define INTERRUPT_SCI0      15
#define INTERRUPT_CLCD      16
#define INTERRUPT_DMA       17
#define INTERRUPT_PWRFAIL   18
#define INTERRUPT_MBX       19

typedef void(*irq_handler_t)(void);

void irq_enable(uint32_t interrupt, irq_handler_t handler);
void irq_disable(uint32_t interrupt);

static inline int interrupt_mask(int interrupt) {
  return 1 << (interrupt & 31);
}

#define SOFTWARE_INTERRUPT(x) ((*INT_SOFT) = interrupt_mask((x)))

#endif
