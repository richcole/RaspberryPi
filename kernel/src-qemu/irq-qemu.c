#include "types.h"

struct irq_t;
struct irq_t volatile *irq = (struct irq_t volatile *) 0x10140000;
