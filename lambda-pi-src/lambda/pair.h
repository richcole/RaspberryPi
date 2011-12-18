#ifndef PAIR_H
#define PAIR_H
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

#define CAR(obj)                            (((scm_pair_t)(obj))->car)
#define CDR(obj)                            (((scm_pair_t)(obj))->cdr)
#define CAAR(obj)                           (CAR(CAR(obj)))
#define CADR(obj)                           (CAR(CDR(obj)))
#define CDAR(obj)                           (CDR(CAR(obj)))
#define CDDR(obj)                           (CDR(CDR(obj)))
#define CADDR(obj)                          (CAR(CDR(CDR(obj))))
#define CADAR(obj)                          (CAR(CDR(CAR(obj))))
#define CDDDR(obj)                          (CDR(CDR(CDR(obj))))

#define SET_CAR(pair,obj)                   CAR(pair)=((scm_obj_t)(obj))
#define SET_CDR(pair,obj)                   CDR(pair)=((scm_obj_t)(obj))

static inline scm_obj_t make_pair(scm_obj_t car, scm_obj_t cdr) {
  scm_obj_t obj = alloc_cells(2);
  CAR(obj) = car;
  CDR(obj) = cdr;
  return obj;
}

static inline scm_obj_t nconc(scm_obj_t * lhs, scm_obj_t rhs) {
  if (*lhs == scm_nil) {
    *lhs = rhs;
  } else {
    scm_obj_t pair = *lhs;
    while (PAIRP(CDR(pair))) {
      pair = CDR(pair);
    }
    SET_CDR(pair, rhs);
  }
  return *lhs;
}


#endif /* end of include guard: PAIR_H */
