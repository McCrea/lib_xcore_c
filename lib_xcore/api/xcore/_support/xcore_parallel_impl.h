// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_meta_macro.h>
#include <xcore/_support/xcore_macros.h>
#include <xcore/thread.h>


#define _XCORE_PAR_PFUNC_FUNC_I(FN, ARG_) FN
#define _XCORE_PAR_PFUNC_FUNC(PACK) _XCORE_PSHIM(_XCORE_PAR_PFUNC_FUNC_I, PACK)

#define _XCORE_PAR_FUNC_STACK_ENTRY_I(FN, _ARGS) \
  ".add_to_set .LPar%=sg," _XCORE_STRINGIFY(FN) ".nstackwords \n\t" \
  ".add_to_set .LPar%=cg," _XCORE_STRINGIFY(FN) ".maxcores \n\t" \
  ".add_to_set .LPar%=tg," _XCORE_STRINGIFY(FN) ".maxtimers \n\t" \
  ".add_to_set .LPar%=eg," _XCORE_STRINGIFY(FN) ".maxchanends \n\t"
#define _XCORE_PAR_FUNC_STACK_ENTRY(PACK) _XCORE_PSHIM(_XCORE_PAR_FUNC_STACK_ENTRY_I, PACK)

#define _XCORE_PAR_STACK_ENTRY_I(PAR_NAME, ...) \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".nstackwords \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".nstackwords.group \n\t" \
  ".set .LPar%=sg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxcores \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxcores.group \n\t" \
  ".set .LPar%=cg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxtimers \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxtimers.group \n\t" \
  ".set .LPar%=tg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxchanends \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxchanends.group \n\t" \
  ".set .LPar%=eg,0 \n\t" \
  _XCORE_APPLY_NOSEP(_XCORE_PAR_FUNC_STACK_ENTRY, __VA_ARGS__) \
  ".sum_reduce .LPar%=sm,.LPar%=sg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".nstackwords.group,.LPar%=sm \n\t" \
  ".sum_reduce .LPar%=cm,.LPar%=cg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxcores.group,.LPar%=cm \n\t" \
  ".sum_reduce .LPar%=tm,.LPar%=tg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxtimers.group,.LPar%=tm \n\t" \
  ".sum_reduce .LPar%=em,.LPar%=eg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxchanends.group,.LPar%=em \n" 

#define _XCORE_PAR_STACK_ENTRY(PAR_NAME, ...) \
  asm(_XCORE_PAR_STACK_ENTRY_I(PAR_NAME, __VA_ARGS__) : /* No outputs but we want to use extended ASM so have an empty list */)

#define _XCORE_PAR_LOAD_STACKWORDS_ASM(DEST, NAME) \
   asm("ldc %[dest], " _XCORE_STRINGIFY(NAME) ".nstackwords" : [dest] "=r" (DEST))

#define _XCORE_PAR_PRIME_II(FUNC, ARG) \
  thread_group_add(__xcore_par_sync, (void(*)(void *))(FUNC), ARG, __xcore_stackbase); \
  do { \
    register unsigned __xcore_stackwords_child; \
    _XCORE_PAR_LOAD_STACKWORDS_ASM(__xcore_stackwords_child, FUNC); \
    __xcore_stackbase = __xcore_substack_advance(__xcore_stackbase, __xcore_stackwords_child); \
  } while(0);

#define _XCORE_PAR_PRIME_I(FUNC, ARG) _XCORE_PAR_PRIME_II(FUNC, (ARG))
#define _XCORE_PAR_PRIME(PACK) _XCORE_PSHIM(_XCORE_PAR_PRIME_I, PACK)

#define _XCORE_PAR_SYNCEXEC_I(FUNC, ARG) do { (FUNC)((ARG)); } while (0)
#define _XCORE_PAR_SYNCEXEC(PACK) _XCORE_PSHIM(_XCORE_PAR_SYNCEXEC_I, PACK)

#define _XCORE_PAR_FUNCS_I(GROUP_NAME, FIRST, ...) \
  do { \
    _XCORE_PAR_STACK_ENTRY(GROUP_NAME, FIRST, __VA_ARGS__); \
    register void *__xcore_stackbase; \
    do { \
      register unsigned __xcore_stackwords_parent; \
      _XCORE_PAR_LOAD_STACKWORDS_ASM(__xcore_stackwords_parent, _XCORE_PAR_PFUNC_FUNC(FIRST)); \
      register void *__xcore_sp; \
      asm volatile("ldaw %[sp], sp[0]" : [sp] "=r" (__xcore_sp)); \
      __xcore_stackbase = __xcore_substack_advance(__xcore_sp, __xcore_stackwords_parent); \
    } while (0); \
    const threadgroup_t __xcore_par_sync = __xcore_allocate_thread_group(); \
    _XCORE_APPLY_NOSEP(_XCORE_PAR_PRIME, __VA_ARGS__) \
    thread_group_start(__xcore_par_sync); \
    _XCORE_PAR_SYNCEXEC(FIRST); \
    if (__builtin_expect(__xcore_dynamically_false(), 0)) \
    { \
      __attribute__(( fptrgroup(_XCORE_STRINGIFY(GROUP_NAME)) )) void (* volatile __xcore_f)(void) = 0; \
      __xcore_f(); \
    } \
    thread_group_wait_and_free(__xcore_par_sync); \
  } while (0)


#define _XCORE_PICK_IF_VOID_I__void 0,
#define _XCORE_PICK_IF_VOID_III(A_, CHOSEN, ...) CHOSEN
#define _XCORE_PICK_IF_VOID_II(PICKER, NEXT) _XCORE_PICK_IF_VOID_III(PICKER, NEXT)
#define _XCORE_PICK_IF_VOID_I(TEST, IF_VOID, ELSE) _XCORE_PICK_IF_VOID_II(_XCORE_PICK_IF_VOID_I__ ## TEST IF_VOID, ELSE)
#define _XCORE_PICK_IF_VOID(TEST, IF_VOID, ELSE) _XCORE_PICK_IF_VOID_I(TEST, IF_VOID, ELSE)

#define _XCORE_PFIRST_I(F, ...) F
#define _XCORE_PFIRST(PACK) _XCORE_PSHIM(_XCORE_PFIRST_I, PACK, )
#define _XCORE_IF_VOID_PACK(PACK, IF_VOID, ELSE) _XCORE_PICK_IF_VOID(_XCORE_PFIRST(PACK), IF_VOID, ELSE)

#define _XCORE_PAR_ARG_STRUCT_NAME_I(FNAME) __xcore_ugs_ ## FNAME ## __xcore_args
#define _XCORE_PAR_ARG_STRUCT_NAME(FNAME) _XCORE_PAR_ARG_STRUCT_NAME_I(FNAME)

#define _XCORE_PAR_UNIQUE_ARG_LABEL(_) _XCORE_UNIQUE_LABEL(__xcore_parg)
#define _XCORE_PAR_PACK_ASSIGN_NAME(ATYPE) (ATYPE, _XCORE_PAR_UNIQUE_ARG_LABEL(ATYPE))
#define _XCORE_PAR_ARG_PACK_PREPARE_I(...) _XCORE_APPLY(_XCORE_PAR_PACK_ASSIGN_NAME, ## __VA_ARGS__)
#define _XCORE_PAR_ARG_PACK_PREPARE(PACK) (_XCORE_PAR_ARG_PACK_PREPARE_I PACK)

#define _XCORE_PAR_ARG_PACK_STRUCT_ENTRY_I(TYPE, UNAME) TYPE _XCORE_PICK_IF_VOID(TYPE,,UNAME)
#define _XCORE_PAR_ARG_PACK_STRUCT_ENTRY(PACK) _XCORE_PAR_ARG_PACK_STRUCT_ENTRY_I PACK
#define _XCORE_PAR_ARG_PACK_STRUCT_ENTRIES(...) _XCORE_APPLY_I(_XCORE_PAR_ARG_PACK_STRUCT_ENTRY, _XCORE_SEP_SEMICOLON, __VA_ARGS__)
#define _XCORE_PAR_ARG_PACK_PARAM_LIST(...) _XCORE_APPLY(_XCORE_PAR_ARG_PACK_STRUCT_ENTRY, __VA_ARGS__)

#define _XCORE_PAR_ARG_PACK_EXTRACT_ARG_I(TYPE, UNAME) _XCORE_PICK_IF_VOID(TYPE,, __xcore_pargs->UNAME)
#define _XCORE_PAR_ARG_PACK_EXTRACT_ARG(PACK) _XCORE_PAR_ARG_PACK_EXTRACT_ARG_I PACK
#define _XCORE_PAR_ARG_PACK_EXTRACT_PACK(...) _XCORE_APPLY(_XCORE_PAR_ARG_PACK_EXTRACT_ARG,  __VA_ARGS__)

#define DECLARE_JOB_I(FNAME, STRUCT_NAME, ARG_PACKS, ARG_TYPES) \
  void FNAME ARG_TYPES; \
  struct STRUCT_NAME { \
    _XCORE_IF_VOID_PACK(ARG_TYPES, ,_XCORE_PSHIM(_XCORE_PAR_ARG_PACK_STRUCT_ENTRIES, ARG_PACKS)); \
  }; \
  /* TODO: Factor out names! */ \
  __attribute__((unused)) \
  static void __xcore_ugs_shim_ ## FNAME  (void *__xcore_pargs_) \
  { \
    const struct STRUCT_NAME * const __xcore_pargs __attribute__((unused)) = __xcore_pargs_; \
    FNAME(_XCORE_PSHIM(_XCORE_PAR_ARG_PACK_EXTRACT_PACK, ARG_PACKS)); \
    asm volatile("" : : : "memory"); \
  } /*\
  static void __xcore_ugs_dispatch_ ## FNAME (_XCORE_PSHIM(_XCORE_PAR_ARG_PACK_PARAM_LIST, ARG_PACKS)) \
  { \
    void *__xcore_pstack = memalign(4, SSIZE * 4);\
    struct STRUCT_NAME __xcore_pargs = {};\
    free(__xcore_pstack); \
  }*/

    // register void *__xcore_sp; \
    // asm volatile("ldaw %[sp], sp[0]" : [sp] "=r" (__xcore_sp)); \
    // printf("My stack pointer is: %p\n", __xcore_sp); \

// #define DECLARE_JOB_VOID(FNAME, STRUCT_NAME, ARG_PACKS, ARG_TYPES, SSIZE)
//   void FNAME (void); \
//   /* TODO: Factor out names! */ \
//   static const size_t __xcore_gsu_swords_ ## FNAME = (SSIZE); \
//   static void __xcore_ugs_shim_ ## FNAME  (void) \
//   { \
//     FNAME(); \
//     asm volatile("" : : : "memory"); \
//   }
 
// #define DECLARE_JOB_I(FNAME, STRUCT_NAME, ARG_PACKS, ARG_TYPES, SSIZE) \
//     _XCORE_IF_VOID_PACK(ARG_TYPES, DECLARE_JOB_VOID, DECLARE_JOB_NONVOID) (\
//         FNAME, STRUCT_NAME, ARG_PACKS, ARG_TYPES, SSIZE) 

_XCORE_EXFUN
inline void *__xcore_substack_advance(void * const base, const unsigned headroom_words)
{
  register void *result;
  asm("ldaw %[result], %[base][-%[words]]"
      : [result] "=r" (result)
      : [base] "r" (base)
      , [words] "r" (headroom_words));
  return result;
  //return ((char *)base) - sizeof(int)*headroom_words; 
}

_XCORE_EXFUN
inline int __xcore_dynamically_false(void)
{
  register int df;
  asm("ldc %[dest], 0" : [dest] "=r" (df));
  return df;
}

#define _XCORE_JPAR_LOAD_STACKWORDS_ASM(DEST, NAME) \
  asm("ldc %[dest], " _XCORE_STRINGIFY(NAME) ".nstackwords" : [dest] "=r" (DEST))

#define _XCORE_JPAR_DISPATCH_II(FUNC, ARGS, ANAME) \
  const struct _XCORE_PAR_ARG_STRUCT_NAME(FUNC) ANAME = {_XCORE_UNPACK(ARGS)}; \
  thread_group_add(__xcore_par_sync, \
                   (__xcore_ugs_shim_ ## FUNC), \
                   (void *)&ANAME, \
                   __xcore_stackbase); \
  do { \
    register unsigned __xcore_stackwords_child; \
    _XCORE_JPAR_LOAD_STACKWORDS_ASM(__xcore_stackwords_child, __xcore_ugs_shim_ ## FUNC); \
    __xcore_stackbase = __xcore_substack_advance(__xcore_stackbase, __xcore_stackwords_child); \
  } while(0);

#define _XCORE_JPAR_DISPATCH_I(FUNC, ARG, ANAME, SNAME) _XCORE_JPAR_DISPATCH_II(FUNC, ARG, ANAME)
#define _XCORE_JPAR_DISPATCH(PACK) _XCORE_PSHIM(_XCORE_JPAR_DISPATCH_I, PACK, _XCORE_UNIQUE_LABEL(__xcore_pargs), _XCORE_UNIQUE_LABEL(__xcore_substack))

#define _XCORE_JPAR_PJOB_FUNC_I(FN, ARGS_) FN
#define _XCORE_JPAR_PJOB_FUNC(PACK) _XCORE_PSHIM(_XCORE_JPAR_PJOB_FUNC_I, PACK)

#define _XCORE_JPAR_FUNC_STACK_ENTRY(FN) \
  ".add_to_set .LPar%=sg," _XCORE_STRINGIFY(FN) ".nstackwords \n\t" \
  ".add_to_set .LPar%=cg," _XCORE_STRINGIFY(FN) ".maxcores \n\t" \
  ".add_to_set .LPar%=tg," _XCORE_STRINGIFY(FN) ".maxtimers \n\t" \
  ".add_to_set .LPar%=eg," _XCORE_STRINGIFY(FN) ".maxchanends \n\t"


#define _XCORE_JPAR_FUNC_SHIM_STACK_ENTRY_I(FN, _ARGS) _XCORE_JPAR_FUNC_STACK_ENTRY(__xcore_ugs_shim_ ## FN)
#define _XCORE_JPAR_FUNC_SHIM_STACK_ENTRY(PACK) _XCORE_PSHIM(_XCORE_JPAR_FUNC_SHIM_STACK_ENTRY_I, PACK)

#define _XCORE_JPAR_STACK_ENTRY_I(PAR_NAME, FIRST, ...) \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".nstackwords \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".nstackwords.group \n\t" \
  ".set .LPar%=sg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxcores \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxcores.group \n\t" \
  ".set .LPar%=cg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxtimers \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxtimers.group \n\t" \
  ".set .LPar%=tg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxchanends \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxchanends.group \n\t" \
  ".set .LPar%=eg,0 \n\t" \
  _XCORE_APPLY_NOSEP(_XCORE_JPAR_FUNC_SHIM_STACK_ENTRY, __VA_ARGS__) \
  _XCORE_JPAR_FUNC_STACK_ENTRY(_XCORE_JPAR_PJOB_FUNC(FIRST)) \
  ".sum_reduce .LPar%=sm,.LPar%=sg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".nstackwords.group,.LPar%=sm \n\t" \
  ".sum_reduce .LPar%=cm,.LPar%=cg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxcores.group,.LPar%=cm \n\t" \
  ".sum_reduce .LPar%=tm,.LPar%=tg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxtimers.group,.LPar%=tm \n\t" \
  ".sum_reduce .LPar%=em,.LPar%=eg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(PAR_NAME) ".maxchanends.group,.LPar%=em \n" 

#define _XCORE_JPAR_STACK_ENTRY(PAR_NAME, ...) \
  asm(_XCORE_JPAR_STACK_ENTRY_I(PAR_NAME, __VA_ARGS__) : /* No outputs but we want to use extended ASM so have an empty list */)

#define _XCORE_JPAR_SYNCEXEC_I(FUNC, ARG_PACK) do { (FUNC) ARG_PACK; } while (0)
#define _XCORE_JPAR_SYNCEXEC(PACK) _XCORE_PSHIM(_XCORE_JPAR_SYNCEXEC_I, PACK)

#define _XCORE_JPAR_JOBS_I(GROUP_NAME, FIRST, ...) \
  do { \
    _XCORE_JPAR_STACK_ENTRY(GROUP_NAME, FIRST, __VA_ARGS__); \
    register void *__xcore_stackbase; \
    do { \
      register unsigned __xcore_stackwords_parent; \
      _XCORE_JPAR_LOAD_STACKWORDS_ASM(__xcore_stackwords_parent, _XCORE_JPAR_PJOB_FUNC(FIRST)); \
      register void *__xcore_sp; \
      asm volatile("ldaw %[sp], sp[0]" : [sp] "=r" (__xcore_sp)); \
      __xcore_stackbase = __xcore_substack_advance(__xcore_sp, __xcore_stackwords_parent); \
    } while (0); \
    const threadgroup_t __xcore_par_sync = __xcore_allocate_thread_group(); \
    _XCORE_APPLY_NOSEP(_XCORE_JPAR_DISPATCH, ## __VA_ARGS__) \
    thread_group_start(__xcore_par_sync); \
    _XCORE_JPAR_SYNCEXEC(FIRST); \
    if (__builtin_expect(__xcore_dynamically_false(), 0)) \
    { \
      __attribute__(( fptrgroup(_XCORE_STRINGIFY(GROUP_NAME)) )) void (* volatile __xcore_f)(void) = 0; \
      __xcore_f(); \
      __builtin_unreachable(); \
    } \
    thread_group_wait_and_free(__xcore_par_sync); \
  } while (0)
