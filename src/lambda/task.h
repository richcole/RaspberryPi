#ifndef TASK_H
#define TASK_H
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

#include "lambda.h"

typedef void(*task_entry_point_t)(void);

typedef enum {
  TASK_RUNNABLE,
  TASK_SLEEP,
} task_state_t;

#define TASK_SP(hdr)            (*((uint32_t **)(hdr) + 1))
#define TASK_PRIORITY(hdr)      (*((uint32_t **)(hdr) + 2))
#define TASK_STACK(hdr)         (*((uint32_t **)(hdr) + 3))
#define TASK_STATE(hdr)         (*(uint32_t *)(hdr) >> 8)

scm_obj_t make_task(scm_obj_t entry_point, scm_fixnum_t stack_size, scm_fixnum_t priority, scm_fixnum_t state);

scm_obj_t find_next_runnable_task();
scm_obj_t remove_task_from_queue(scm_obj_t task, scm_obj_t * queue);
scm_obj_t add_task_to_queue(scm_obj_t task, scm_obj_t * queue);
scm_obj_t set_task_state(scm_obj_t task, scm_obj_t state);
scm_obj_t set_task_priority(scm_obj_t task, scm_obj_t priority);
void init_task_queues();
#endif /* end of include guard: TASK_H */
