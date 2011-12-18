#include "stdint.h"
#include "interrupts.h"
#include "macros.h"

irq_handler_t __irq_handlers[32];

void irq_enable(uint32_t interrupt, irq_handler_t handler) {
	uint32_t *reg = INT_ENABLE;
	*reg |= interrupt_mask(interrupt);
	__irq_handlers[interrupt] = handler;
}

void irq_disable(uint32_t interrupt) {
	uint32_t *reg = INT_DISABLE;
	*reg |= interrupt_mask(interrupt);
	__irq_handlers[interrupt] = 0L;
}
