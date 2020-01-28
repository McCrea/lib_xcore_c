// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_meta_macro.h>
#include <xcore/thread.h>

#define _XCORE_PAR_WSIZE 4

#define _XCORE_PAR_PRIME_II(FUNC, ARG, SSIZE, SNAME) \
  char SNAME[_XCORE_PAR_WSIZE * SSIZE] __attribute__ ((aligned(_XCORE_PAR_WSIZE))); \
  thread_group_add(__xcore_par_sync, FUNC, ARG, SNAME + _XCORE_PAR_WSIZE * (SSIZE - 1));

#define _XCORE_PAR_PRIME_I(FUNC, ARG, SSIZE, SNAME) _XCORE_PAR_PRIME_II((FUNC), (ARG), (SSIZE), SNAME)
#define _XCORE_PAR_PRIME(PACK) _XCORE_PSHIM(_XCORE_PAR_PRIME_I, PACK, _XCORE_UNIQUE_LABEL(__substack))

#define _XCORE_PAR_SYNCEXEC_I(FUNC, ARG, SSIZE_) do { (FUNC)((ARG)); } while (0)
#define _XCORE_PAR_SYNCEXEC(PACK) _XCORE_PSHIM(_XCORE_PAR_SYNCEXEC_I, PACK)

#define _XCORE_PAR_FUNCS_I(FIRST, ...) \
  do { \
    const threadgroup_t __xcore_par_sync = __xcore_allocate_thread_group(); \
    _XCORE_APPLY_NOSEP(_XCORE_PAR_PRIME, __VA_ARGS__) \
    thread_group_start(__xcore_par_sync); \
    _XCORE_PAR_SYNCEXEC(FIRST); \
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

#define DECLARE_JOB_I(FNAME, STRUCT_NAME, ARG_PACKS, ARG_TYPES, SSIZE) \
  void FNAME ARG_TYPES; \
  struct STRUCT_NAME { \
    _XCORE_PSHIM(_XCORE_PAR_ARG_PACK_STRUCT_ENTRIES, ARG_PACKS) ; \
  }; \
  /* TODO: Factor out names! */ \
  static const size_t __xcore_gsu_swords_ ## FNAME = (SSIZE); \
  static void __xcore_ugs_shim_ ## FNAME  (void *__xcore_pargs_) \
  { \
    struct STRUCT_NAME * const __xcore_pargs = __xcore_pargs_; \
    FNAME(_XCORE_PSHIM(_XCORE_PAR_ARG_PACK_EXTRACT_PACK, ARG_PACKS)); \
    asm volatile("" : : : "memory"); \
  } /*\
  static void __xcore_ugs_dispatch_ ## FNAME (_XCORE_PSHIM(_XCORE_PAR_ARG_PACK_PARAM_LIST, ARG_PACKS)) \
  { \
    void *__xcore_pstack = memalign(4, SSIZE * 4);\
    struct STRUCT_NAME __xcore_pargs = {};\
    free(__xcore_pstack); \
  }*/

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

#define _XCORE_JPAR_DISPATCH_II(FUNC, ARGS, SSIZE, ANAME, SNAME) \
  struct _XCORE_PAR_ARG_STRUCT_NAME(FUNC) ANAME = {_XCORE_UNPACK(ARGS)}; \
  char SNAME[_XCORE_PAR_WSIZE * SSIZE] __attribute__ ((aligned(_XCORE_PAR_WSIZE))); \
  thread_group_add(__xcore_par_sync, (__xcore_ugs_shim_ ## FUNC), &ANAME, SNAME + _XCORE_PAR_WSIZE * (SSIZE - 1));

#define _XCORE_JPAR_DISPATCH_I(FUNC, ARG, ANAME, SNAME) _XCORE_JPAR_DISPATCH_II(FUNC, ARG, (__xcore_gsu_swords_ ## FUNC), ANAME, SNAME)
#define _XCORE_JPAR_DISPATCH(PACK) _XCORE_PSHIM(_XCORE_JPAR_DISPATCH_I, PACK, _XCORE_UNIQUE_LABEL(__xcore_pargs), _XCORE_UNIQUE_LABEL(__xcore_substack))

#define _XCORE_JPAR_SYNCEXEC_I(FUNC, ARG_PACK) do { (FUNC) ARG_PACK; } while (0)
#define _XCORE_JPAR_SYNCEXEC(PACK) _XCORE_PSHIM(_XCORE_JPAR_SYNCEXEC_I, PACK)

#define _XCORE_JPAR_JOBS_I(FIRST, ...) \
  do { \
    const threadgroup_t __xcore_par_sync = __xcore_allocate_thread_group(); \
    _XCORE_APPLY_NOSEP(_XCORE_JPAR_DISPATCH, ## __VA_ARGS__) \
    thread_group_start(__xcore_par_sync); \
    _XCORE_JPAR_SYNCEXEC(FIRST); \
    thread_group_wait_and_free(__xcore_par_sync); \
  } while (0)
