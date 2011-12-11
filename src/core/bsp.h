#ifndef BSP_H
#define BSP_H
/*        Copyright (c) 20011, Simon Stapleton (simon.stapleton@gmail.com)        */
/*                                                                                */
/*                              All rights reserved.                              */
/*                                                                                */
/* Redistribution  and  use   in  source  and  binary  forms,   with  or  without */
/* modification, are permitted provided that the following conditions are met:    */
/*                                                                                */
/* Redistributions of  source code must  retain the above copyright  notice, this */
/* list of conditions and the following disclaimer.                               */
/*                                                                                */
/* Redistributions in binary form must reproduce the above copyright notice, this */
/* list of conditions and the following disclaimer in the documentation and/or    */
/* other materials provided with the distribution.                                */
/*                                                                                */
/* Neither the name of  the developer nor the names of  other contributors may be */
/* used  to  endorse or  promote  products  derived  from this  software  without */
/* specific prior written permission.                                             */
/*                                                                                */
/* THIS SOFTWARE  IS PROVIDED BY THE  COPYRIGHT HOLDERS AND CONTRIBUTORS  "AS IS" */
/* AND ANY  EXPRESS OR  IMPLIED WARRANTIES,  INCLUDING, BUT  NOT LIMITED  TO, THE */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE */
/* DISCLAIMED. IN NO  EVENT SHALL THE COPYRIGHT HOLDER OR  CONTRIBUTORS BE LIABLE */
/* FOR  ANY DIRECT,  INDIRECT, INCIDENTAL,  SPECIAL, EXEMPLARY,  OR CONSEQUENTIAL */
/* DAMAGES (INCLUDING,  BUT NOT  LIMITED TO, PROCUREMENT  OF SUBSTITUTE  GOODS OR */
/* SERVICES; LOSS  OF USE,  DATA, OR PROFITS;  OR BUSINESS  INTERRUPTION) HOWEVER */
/* CAUSED AND ON ANY THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  IN ANY WAY OUT OF THE USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           */

#include <lib/stdint.h>
#include <lib/sysmacros.h>
#include <lib/errno.h>

// Systicks
#define SYSTICKS_HZ  1000

// Function entry for OS startup
void platform_startup();

// Global variables we might want to look at
extern const uint32_t *  __memtop;      /* The top of available memory */
extern const uint32_t *  __heap_start;  /* Start of the heap */
extern const uint32_t __system_ram;     /* Amount of system RAM in megabytes */
extern uint32_t * __heap_top;           /* pointer to the current top of the heap */

#include "platform.h"

typedef void(*irq_handler_t)(void);

void irq_enable(uint32_t interrupt, irq_handler_t handler);
void irq_disable(uint32_t interrupt);

uint32_t first_set_bit(uint32_t);

void tick();

#include "irq.h"
#include "syscalls.h"
#include "mutex.h"
#include "sleep.h"

#include "alloc.h"

#include "sp804.h"
#include "pl011.h"


#endif /* end of include guard: BSP_H */
