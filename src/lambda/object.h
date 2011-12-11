#ifndef OBJECT_TYPES_H
#define OBJECT_TYPES_H
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

#include <stdint.h>
#include <errno.h>

// Type encodings
//
// pppp pppp pppp pppp pppp pppp pppp p000 : scm_cell_t
// nnnn nnnn nnnn nnnn nnnn nnnn nnnn 0001 : scm_fixnum_t
// cccc cccc cccc cccc cccc cccc .... 0010 : scm_char_t
// cccc cccc cccc cccc cccc cccc cccc 0011 : scm_const_t
//
// Boxed types
// nnnn nnnn nnnn nnnn nnnn nnnn 0000 1010 : scm_hdr_task       n - Task state

#define OBJECT_DATUM_ALIGN              8
#define OBJECT_DATUM_ALIGN_MASK         (OBJECT_DATUM_ALIGN - 1)

typedef uint32_t *          scm_obj_t;
typedef uint32_t            scm_hdr_t;
typedef scm_obj_t           scm_cell_t;
typedef scm_obj_t           scm_fixnum_t;
typedef scm_obj_t           scm_char_t;
typedef scm_obj_t           scm_task_t;
typedef scm_obj_t           scm_const_t;

#define T_CELL    0x00
#define T_FIXNUM  0x01
#define T_CHAR    0x02
#define T_CONST   0x03

#define T_HDR 0x0a

#define HDR_TYPE_MASKBITS   0xff

typedef enum {
  BOXED_TASK,
} boxed_type_identifier_t;

static const scm_hdr_t scm_hdr_task = (scm_hdr_t)(T_HDR | (BOXED_TASK << 4));

typedef enum {
  CONST_NIL,
  CONST_TRUE,
  CONST_FALSE,
  CONST_UNDEF,
  CONST_UNSPEC,
  CONST_EOF,
} const_identifier_t;

#define scm_nil (scm_obj_t)(T_CONST | (CONST_NIL << 4))
#define scm_true (scm_obj_t)(T_CONST | (CONST_TRUE << 4))
#define scm_false (scm_obj_t)(T_CONST | (CONST_FALSE << 4))
#define scm_undef (scm_obj_t)(T_CONST | (CONST_UNDEF << 4))
#define scm_unspec (scm_obj_t)(T_CONST | (CONST_UNSPEC << 4))
#define scm_eof (scm_obj_t)(T_CONST | (CONST_EOF << 4))

#define BITS(obj)           ((uint32_t)(obj))
#define HDR(obj)            (*(scm_hdr_t*)(obj))


typedef struct {
  scm_obj_t car;
  scm_obj_t cdr;
} scm_pair_rec_t;

typedef scm_pair_rec_t*     scm_pair_t;

// #define OBJECT_ALIGNED(x)   struct DECLSPEC(align(OBJECT_DATUM_ALIGN)) x
// #define END                 ATTRIBUTE(aligned(OBJECT_DATUM_ALIGN))
// 
// OBJECT_ALIGNED(scm_pair_rec_t) {
//     scm_obj_t   car;
//     scm_obj_t   cdr;
// } END;

#define FIXNUMP(obj)        ((BITS(obj) & 0x0f) == T_FIXNUM)
#define CHARP(obj)          ((BITS(obj) & 0x0f) == T_CHAR)
#define CELLP(obj)          ((BITS(obj) & 0x07) == T_CELL)
#define PAIRP(obj)          ((CELLP(obj) && (HDR(obj) & 0x0f) != T_HDR))
#define CONSTP(obj)         ((BITS(obj) & 0x0f) == T_CONST)

#define TASKP(obj)          (CELLP(obj) && ((HDR(obj) & HDR_TYPE_MASKBITS) == scm_hdr_task))



#endif /* end of include guard: OBJECT_TYPES_H */
