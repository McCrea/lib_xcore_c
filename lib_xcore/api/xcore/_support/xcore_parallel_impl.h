// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_meta_macro.h>
#include <xcore/_support/xcore_macros.h>
#include <xcore/thread.h>


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
#define _XCORE_VOID_PTR_CAST(_PTR, _TYPE) static_cast<_TYPE>(_PTR)
#define _XCORE_CLINKAGE_BEGIN extern "C" {
#define _XCORE_CLINKAGE_END }
#else
#define _XCORE_VOID_PTR_CAST(_PTR, _TYPE) _PTR
#define _XCORE_CLINKAGE_BEGIN
#define _XCORE_CLINKAGE_END
#endif


#define _XCORE_PAR_PFUNC_FUNC_I(_FN, _ARG_) _FN
#define _XCORE_PAR_PFUNC_FUNC(_PACK) _XCORE_PSHIM(_XCORE_PAR_PFUNC_FUNC_I, _PACK)

#define _XCORE_PAR_FUNC_STACK_ENTRY_I(_FN, _ARGS_) \
  ".add_to_set .LPar%=sg,(" _XCORE_STRINGIFY(_FN) ".nstackwords + " _XCORE_STRINGIFY(_XCORE_STACK_ALIGN_REQUIREMENT_WORDS) ") \n\t" \
  ".add_to_set .LPar%=cg," _XCORE_STRINGIFY(_FN) ".maxcores \n\t" \
  ".add_to_set .LPar%=tg," _XCORE_STRINGIFY(_FN) ".maxtimers \n\t" \
  ".add_to_set .LPar%=eg," _XCORE_STRINGIFY(_FN) ".maxchanends \n\t"
#define _XCORE_PAR_FUNC_STACK_ENTRY(_PACK) _XCORE_PSHIM(_XCORE_PAR_FUNC_STACK_ENTRY_I, _PACK)


#define _XCORE_PAR_MASTER_STACK_SYMBOLS_I(_FN_STR, _PAR_NAME_STR) \
  ".set .L" _PAR_NAME_STR "ms,( " \
    _FN_STR ".nstackwords " \
    "$M thread_group_add.nstackwords " \
    "$M __xcore_substack_advance.nstackwords " \
    "$M thread_group_start.nstackwords " \
    "$M __xcore_dynamically_false.nstackwords " \
    "$M thread_group_wait_and_free.nstackwords ) \n\t" \
  ".add_to_set .LPar%=sg,.L" _PAR_NAME_STR "ms \n\t" \
  ".add_to_set .LPar%=cg," _FN_STR ".maxcores \n\t" \
  ".add_to_set .LPar%=tg," _FN_STR ".maxtimers \n\t" \
  ".add_to_set .LPar%=eg," _FN_STR ".maxchanends \n\t"

#define _XCORE_PAR_MASTER_STACK_SYMBOLS(_FN, _PAR_NAME) \
  _XCORE_PAR_MASTER_STACK_SYMBOLS_I(_XCORE_STRINGIFY(_FN), _XCORE_STRINGIFY(_PAR_NAME))

#define _XCORE_PAR_STACK_ENTRY_I(_PAR_NAME, _FIRST, ...) \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".nstackwords \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".nstackwords.group \n\t" \
  ".set .LPar%=sg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxcores \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxcores.group \n\t" \
  ".set .LPar%=cg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxtimers \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxtimers.group \n\t" \
  ".set .LPar%=tg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxchanends \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxchanends.group \n\t" \
  ".set .LPar%=eg,0 \n\t" \
  _XCORE_APPLY_NOSEP(_XCORE_PAR_FUNC_STACK_ENTRY, __VA_ARGS__) \
  _XCORE_PAR_MASTER_STACK_SYMBOLS(_XCORE_PAR_PFUNC_FUNC(_FIRST), _PAR_NAME) \
  ".sum_reduce .LPar%=sm,.LPar%=sg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".nstackwords.group,.LPar%=sm \n\t" \
  ".sum_reduce .LPar%=cm,.LPar%=cg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxcores.group,.LPar%=cm \n\t" \
  ".sum_reduce .LPar%=tm,.LPar%=tg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxtimers.group,.LPar%=tm \n\t" \
  ".sum_reduce .LPar%=em,.LPar%=eg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxchanends.group,.LPar%=em \n" 

#define _XCORE_PAR_STACK_ENTRY(_PAR_NAME, ...) \
  asm(_XCORE_PAR_STACK_ENTRY_I(_PAR_NAME,  __VA_ARGS__) : /* No outputs but we want to use extended ASM so have an empty list */)

#define _XCORE_PAR_LOAD_STACKWORDS_ASM(_DEST, _NAME) \
   asm("ldc %[__dest], " _XCORE_STRINGIFY(_NAME) ".nstackwords" : [__dest] "=r" (_DEST))

#define _XCORE_PAR_PRIME_II(_FUNC, _ARG) \
  thread_group_add(__xcore_par_sync, (void(*)(void *))(_FUNC), _ARG, __xcore_stackbase); \
  do { \
    register unsigned __xcore_stackwords_child; \
    _XCORE_PAR_LOAD_STACKWORDS_ASM(__xcore_stackwords_child, _FUNC); \
    __xcore_stackbase = __xcore_substack_advance(__xcore_stackbase, __xcore_stackwords_child); \
  } while(0);

#define _XCORE_PAR_PRIME_I(_FUNC, _ARG) _XCORE_PAR_PRIME_II(_FUNC, (_ARG))
#define _XCORE_PAR_PRIME(_PACK) _XCORE_PSHIM(_XCORE_PAR_PRIME_I, _PACK)

#define _XCORE_PAR_SYNCEXEC_I(_FUNC, _ARG) do { (_FUNC)((_ARG)); } while (0)
#define _XCORE_PAR_SYNCEXEC(_PACK) _XCORE_PSHIM(_XCORE_PAR_SYNCEXEC_I, _PACK)

#define _XCORE_PAR_FUNCS_I(_GROUP_NAME, _FIRST, ...) \
  do { \
    _XCORE_PAR_STACK_ENTRY(_GROUP_NAME, _FIRST, __VA_ARGS__); \
    register void *__xcore_stackbase; \
    do { \
      register unsigned __xcore_stackwords_parent; \
      asm("ldc %[__dest],.L" _XCORE_STRINGIFY(_GROUP_NAME) "ms \n" \
          : [__dest] "=r" (__xcore_stackwords_parent)); \
      register void *__xcore_sp; \
      asm volatile("ldaw %[__sp], sp[0]" : [__sp] "=r" (__xcore_sp)); \
      __xcore_stackbase = __xcore_substack_advance(__xcore_sp, __xcore_stackwords_parent); \
    } while (0); \
    const threadgroup_t __xcore_par_sync = __xcore_allocate_thread_group(); \
    _XCORE_APPLY_NOSEP(_XCORE_PAR_PRIME, __VA_ARGS__) \
    thread_group_start(__xcore_par_sync); \
    _XCORE_PAR_SYNCEXEC(_FIRST); \
    if (__builtin_expect(__xcore_dynamically_false(), 0)) \
    { \
      __attribute__(( fptrgroup(_XCORE_STRINGIFY(_GROUP_NAME)) )) void (* volatile __xcore_f)(void) = 0; \
      __xcore_f(); \
    } \
    thread_group_wait_and_free(__xcore_par_sync); \
  } while (0)


#define _XCORE_PICK_IF_VOID_I__void 0,
#define _XCORE_PICK_IF_VOID_III(_A_, _CHOSEN, ...) _CHOSEN
#define _XCORE_PICK_IF_VOID_II(_PICKER, _NEXT) _XCORE_PICK_IF_VOID_III(_PICKER, _NEXT)
#define _XCORE_PICK_IF_VOID_I(_TEST, _IF_VOID, _ELSE) _XCORE_PICK_IF_VOID_II(_XCORE_PICK_IF_VOID_I__ ## _TEST _IF_VOID, _ELSE)
#define _XCORE_PICK_IF_VOID(_TEST, _IF_VOID, _ELSE) _XCORE_PICK_IF_VOID_I(_TEST, _IF_VOID, _ELSE)

#define _XCORE_PFIRST_I(_F, ...) _F
#define _XCORE_PFIRST(_PACK) _XCORE_PSHIM(_XCORE_PFIRST_I, _PACK, )
#define _XCORE_IF_VOID_PACK(_PACK, _IF_VOID, _ELSE) _XCORE_PICK_IF_VOID(_XCORE_PFIRST(_PACK), _IF_VOID, _ELSE)

#define _XCORE_PAR_ARG_STRUCT_NAME_I(_FNAME) __xcore_ugs_ ## _FNAME ## __xcore_args
#define _XCORE_PAR_ARG_STRUCT_NAME(_FNAME) _XCORE_PAR_ARG_STRUCT_NAME_I(_FNAME)

#define _XCORE_PAR_UNIQUE_ARG_LABEL(_) _XCORE_UNIQUE_LABEL(__xcore_parg)
#define _XCORE_PAR_PACK_ASSIGN_NAME(_ATYPE) (_ATYPE, _XCORE_PAR_UNIQUE_ARG_LABEL(_ATYPE))
#define _XCORE_PAR_ARG_PACK_PREPARE_I(...) _XCORE_APPLY(_XCORE_PAR_PACK_ASSIGN_NAME, ## __VA_ARGS__)
#define _XCORE_PAR_ARG_PACK_PREPARE(_PACK) (_XCORE_PAR_ARG_PACK_PREPARE_I _PACK)

#define _XCORE_PAR_ARG_PACK_STRUCT_ENTRY_I(_TYPE, _UNAME) _TYPE _XCORE_PICK_IF_VOID(_TYPE,,_UNAME)
#define _XCORE_PAR_ARG_PACK_STRUCT_ENTRY(_PACK) _XCORE_PAR_ARG_PACK_STRUCT_ENTRY_I _PACK
#define _XCORE_PAR_ARG_PACK_STRUCT_ENTRIES(...) _XCORE_APPLY_I(_XCORE_PAR_ARG_PACK_STRUCT_ENTRY, _XCORE_SEP_SEMICOLON, __VA_ARGS__)
#define _XCORE_PAR_ARG_PACK_PARAM_LIST(...) _XCORE_APPLY(_XCORE_PAR_ARG_PACK_STRUCT_ENTRY, __VA_ARGS__)

#define _XCORE_PAR_ARG_PACK_EXTRACT_ARG_I(_TYPE, _UNAME) _XCORE_PICK_IF_VOID(_TYPE,, __xcore_pargs->_UNAME)
#define _XCORE_PAR_ARG_PACK_EXTRACT_ARG(_PACK) _XCORE_PAR_ARG_PACK_EXTRACT_ARG_I _PACK
#define _XCORE_PAR_ARG_PACK_EXTRACT_PACK(...) _XCORE_APPLY(_XCORE_PAR_ARG_PACK_EXTRACT_ARG,  __VA_ARGS__)

#define DECLARE_JOB_I(_FNAME, _STRUCT_NAME, _ARG_PACKS, _ARG_TYPES) \
  void _FNAME _ARG_TYPES _XCORE_CFUNC_EXCEPT_SPEC asm(_XCORE_STRINGIFY(_FNAME)); \
  struct _STRUCT_NAME { \
    _XCORE_IF_VOID_PACK(_ARG_TYPES, ,_XCORE_PSHIM(_XCORE_PAR_ARG_PACK_STRUCT_ENTRIES, _ARG_PACKS)); \
  }; \
  /* TODO: Factor out names! */ \
  static void __xcore_ugs_shim_ ## _FNAME  (void *__xcore_pargs_) _XCORE_NOTHROW asm(_XCORE_STRINGIFY(__xcore_ugs_shim_ ## _FNAME)); \
  __attribute__((unused)) \
  static void __xcore_ugs_shim_ ## _FNAME  (void *__xcore_pargs_) _XCORE_NOTHROW \
  { \
    const struct _STRUCT_NAME * const __xcore_pargs __attribute__((unused)) = \
      _XCORE_VOID_PTR_CAST(__xcore_pargs_, const struct _STRUCT_NAME *); \
    _FNAME(_XCORE_PSHIM(_XCORE_PAR_ARG_PACK_EXTRACT_PACK, _ARG_PACKS)); \
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
inline void *__xcore_substack_advance(void * const __base, const unsigned __headroom_words)
{
  register void *__result;
  asm("ldaw %[__result], %[__base][-%[__words]]"
      : [__result] "=r" (__result)
      : [__base] "r" (__base)
      , [__words] "r" (__headroom_words + _XCORE_STACK_ALIGN_REQUIREMENT_WORDS));
  return __result;
  //return ((char *)base) - sizeof(int)*headroom_words; 
}

_XCORE_EXFUN
inline __xcore_bool_t __xcore_dynamically_false(void)
{
  register int __df;
  asm("ldc %[__dest], 0"
      : [__dest] "=r" (__df));
  return __df;
}

#define _XCORE_JPAR_LOAD_STACKWORDS_ASM(_DEST, _NAME) \
  asm("ldc %[__dest], " _XCORE_STRINGIFY(_NAME) ".nstackwords" \
      : [__dest] "=r" (_DEST))

#define _XCORE_JPAR_DISPATCH_II(_FUNC, _ARGS, _ANAME) \
  const struct _XCORE_PAR_ARG_STRUCT_NAME(_FUNC) _ANAME = {_XCORE_UNPACK(_ARGS)}; \
  thread_group_add(__xcore_par_sync, \
                   (__xcore_ugs_shim_ ## _FUNC), \
                   (void *)&_ANAME, \
                   __xcore_stackbase); \
  do { \
    register unsigned __xcore_stackwords_child; \
    _XCORE_JPAR_LOAD_STACKWORDS_ASM(__xcore_stackwords_child, __xcore_ugs_shim_ ## _FUNC); \
    __xcore_stackbase = __xcore_substack_advance(__xcore_stackbase, __xcore_stackwords_child); \
  } while(0);

#define _XCORE_JPAR_DISPATCH_I(_FUNC, _ARG, _ANAME, _SNAME) _XCORE_JPAR_DISPATCH_II(_FUNC, _ARG, _ANAME)
#define _XCORE_JPAR_DISPATCH(_PACK) _XCORE_PSHIM(_XCORE_JPAR_DISPATCH_I, _PACK, _XCORE_UNIQUE_LABEL(__xcore_pargs), _XCORE_UNIQUE_LABEL(__xcore_substack))

#define _XCORE_JPAR_PJOB_FUNC_I(_FN, _ARGS_) _FN
#define _XCORE_JPAR_PJOB_FUNC(_PACK) _XCORE_PSHIM(_XCORE_JPAR_PJOB_FUNC_I, _PACK)

#define _XCORE_JPAR_FUNC_STACK_ENTRY(_FN) \
  ".add_to_set .LPar%=sg,(" _XCORE_STRINGIFY(_FN) ".nstackwords + " _XCORE_STRINGIFY(_XCORE_STACK_ALIGN_REQUIREMENT_WORDS) ") \n\t" \
  ".add_to_set .LPar%=cg," _XCORE_STRINGIFY(_FN) ".maxcores \n\t" \
  ".add_to_set .LPar%=tg," _XCORE_STRINGIFY(_FN) ".maxtimers \n\t" \
  ".add_to_set .LPar%=eg," _XCORE_STRINGIFY(_FN) ".maxchanends \n\t"

					
#define _XCORE_JPAR_MASTER_STACK_SYMBOLS_II(_FN_STR, _PAR_NAME_STR) \
  ".set .L" _PAR_NAME_STR "ms,( " \
    _FN_STR ".nstackwords " \
    "$M thread_group_add.nstackwords " \
    "$M __xcore_substack_advance.nstackwords " \
    "$M thread_group_start.nstackwords " \
    "$M __xcore_dynamically_false.nstackwords " \
    "$M thread_group_wait_and_free.nstackwords ) \n\t" \
  ".add_to_set .LPar%=sg,.L" _PAR_NAME_STR "ms \n\t" \
  ".add_to_set .LPar%=cg," _FN_STR ".maxcores \n\t" \
  ".add_to_set .LPar%=tg," _FN_STR ".maxtimers \n\t" \
  ".add_to_set .LPar%=eg," _FN_STR ".maxchanends \n\t"

#define _XCORE_JPAR_MASTER_STACK_SYMBOLS_I(_FN, _PAR_NAME_STR) \
  _XCORE_JPAR_MASTER_STACK_SYMBOLS_II(_XCORE_STRINGIFY(_FN), _PAR_NAME_STR)

#define _XCORE_JPAR_MASTER_STACK_SYMBOLS(_FN, _PAR_NAME) \
  _XCORE_JPAR_MASTER_STACK_SYMBOLS_I(_FN, _XCORE_STRINGIFY(_PAR_NAME))

#define _XCORE_JPAR_FUNC_SHIM_STACK_ENTRY_I(_FN, _ARGS_) _XCORE_JPAR_FUNC_STACK_ENTRY(__xcore_ugs_shim_ ## _FN)
#define _XCORE_JPAR_FUNC_SHIM_STACK_ENTRY(_PACK) _XCORE_PSHIM(_XCORE_JPAR_FUNC_SHIM_STACK_ENTRY_I, _PACK)

#define _XCORE_JPAR_STACK_ENTRY_I(_PAR_NAME, _FIRST, ...) \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".nstackwords \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".nstackwords.group \n\t" \
  ".set .LPar%=sg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxcores \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxcores.group \n\t" \
  ".set .LPar%=cg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxtimers \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxtimers.group \n\t" \
  ".set .LPar%=tg,0 \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxchanends \n\t" \
  ".local _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxchanends.group \n\t" \
  ".set .LPar%=eg,0 \n\t" \
  _XCORE_APPLY_NOSEP(_XCORE_JPAR_FUNC_SHIM_STACK_ENTRY, __VA_ARGS__) \
  _XCORE_JPAR_MASTER_STACK_SYMBOLS(_XCORE_JPAR_PJOB_FUNC(_FIRST), _PAR_NAME) \
  ".sum_reduce .LPar%=sm,.LPar%=sg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".nstackwords.group,.LPar%=sm \n\t" \
  ".sum_reduce .LPar%=cm,.LPar%=cg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxcores.group,.LPar%=cm \n\t" \
  ".sum_reduce .LPar%=tm,.LPar%=tg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxtimers.group,.LPar%=tm \n\t" \
  ".sum_reduce .LPar%=em,.LPar%=eg,0 \n\t" \
  ".add_to_set _fptrgroup." _XCORE_STRINGIFY(_PAR_NAME) ".maxchanends.group,.LPar%=em \n" 

#define _XCORE_JPAR_STACK_ENTRY(_PAR_NAME, ...) \
  asm(_XCORE_JPAR_STACK_ENTRY_I(_PAR_NAME, __VA_ARGS__) : /* No outputs but we want to use extended ASM so have an empty list */)

#define _XCORE_JPAR_SYNCEXEC_I(_FUNC, _ARG_PACK) do { (_FUNC) _ARG_PACK; } while (0)
#define _XCORE_JPAR_SYNCEXEC(_PACK) _XCORE_PSHIM(_XCORE_JPAR_SYNCEXEC_I, _PACK)

#ifdef __cplusplus
#define _XCORE_NOTHROW_SITE_START []() __attribute__((noinline)) noexcept {
#define _XCORE_NOTHROW_SITE_END }();
#else
#define _XCORE_NOTHROW_SITE_START
#define _XCORE_NOTHROW_SITE_END
#endif

#define _XCORE_JPAR_JOBS_I(_GROUP_NAME, _FIRST, ...) \
  do { \
    _XCORE_JPAR_STACK_ENTRY(_GROUP_NAME, _FIRST, __VA_ARGS__); \
    register void *__xcore_stackbase; \
    do { \
      register unsigned __xcore_stackwords_parent; \
      asm("ldc %[dest],.L" _XCORE_STRINGIFY(_GROUP_NAME) "ms \n" \
	  : [dest] "=r" (__xcore_stackwords_parent)); \
      register void *__xcore_sp; \
      asm volatile("ldaw %[__sp], sp[0]" : [__sp] "=r" (__xcore_sp)); \
      __xcore_stackbase = __xcore_substack_advance(__xcore_sp, __xcore_stackwords_parent); \
    } while (0); \
    const threadgroup_t __xcore_par_sync = __xcore_allocate_thread_group(); \
    _XCORE_APPLY_NOSEP(_XCORE_JPAR_DISPATCH, ## __VA_ARGS__) \
    thread_group_start(__xcore_par_sync); \
    _XCORE_JPAR_SYNCEXEC(_FIRST); \
    if (__builtin_expect(__xcore_dynamically_false(), 0)) \
    { \
      _XCORE_NOTHROW_SITE_START \
      __attribute__(( fptrgroup(_XCORE_STRINGIFY(_GROUP_NAME)) )) void (* volatile __xcore_f)(void) = 0; \
      __xcore_f(); \
      _XCORE_NOTHROW_SITE_END \
      __builtin_unreachable(); \
    } \
    thread_group_wait_and_free(__xcore_par_sync); \
  } while (0)

#ifdef __cplusplus
}
#endif

