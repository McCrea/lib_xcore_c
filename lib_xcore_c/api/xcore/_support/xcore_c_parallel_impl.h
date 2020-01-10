#pragma once

#include <xcore/_support/xcore_c_meta_macro.h>
#include <xcore/thread.h>

#define _XMM_PAR_WSIZE 4

#define _XMM_PAR_PRIME_II(FUNC, ARG, SSIZE, SNAME) \
  char SNAME[_XMM_PAR_WSIZE * SSIZE] __attribute__ ((aligned(_XMM_PAR_WSIZE))); \
  thread_group_add(__xmm_par_sync, FUNC, ARG, SNAME + _XMM_PAR_WSIZE * (SSIZE - 1));

#define _XMM_PAR_PRIME_I(FUNC, ARG, SSIZE, SNAME) _XMM_PAR_PRIME_II((FUNC), (ARG), (SSIZE), SNAME)
#define _XMM_PAR_PRIME(PACK) _XMM_PSHIM(_XMM_PAR_PRIME_I, PACK, _XMM_UNIQUE_LABEL(__substack))

#define _XMM_PAR_SYNCEXEC_I(FUNC, ARG, SSIZE_) do { (FUNC)((ARG)); } while (0)
#define _XMM_PAR_SYNCEXEC(PACK) _XMM_PSHIM(_XMM_PAR_SYNCEXEC_I, PACK)

#define _XMM_PAR_FUNCS_I(FIRST, ...) \
  do { \
    const threadgroup_t __xmm_par_sync = __xcore_c_allocate_thread_group(); \
    _XMM_APPLY_NOSEP(_XMM_PAR_PRIME, __VA_ARGS__) \
    thread_group_start(__xmm_par_sync); \
    _XMM_PAR_SYNCEXEC(FIRST); \
    thread_group_wait_and_free(__xmm_par_sync); \
  } while (0)

#define _XMM_PAR_ARG_STRUCT_NAME_I(FNAME) __xmm_ugs_ ## FNAME ## _args
#define _XMM_PAR_ARG_STRUCT_NAME(FNAME) _XMM_PAR_ARG_STRUCT_NAME_I(FNAME)

#define _XMM_PAR_UNIQUE_ARG_LABEL(_) _XMM_UNIQUE_LABEL(__xmm_parg)
#define _XMM_PAR_PACK_ASSIGN_NAME(ATYPE) (ATYPE, _XMM_PAR_UNIQUE_ARG_LABEL(ATYPE))
#define _XMM_PAR_ARG_PACK_PREPARE_I(...) _XMM_APPLY(_XMM_PAR_PACK_ASSIGN_NAME, ## __VA_ARGS__)
#define _XMM_PAR_ARG_PACK_PREPARE(PACK) (_XMM_PAR_ARG_PACK_PREPARE_I PACK)

#define _XMM_PAR_ARG_PACK_STRUCT_ENTRY_I(TYPE, UNAME) TYPE UNAME
#define _XMM_PAR_ARG_PACK_STRUCT_ENTRY(PACK) _XMM_PAR_ARG_PACK_STRUCT_ENTRY_I PACK
#define _XMM_PAR_ARG_PACK_STRUCT_ENTRIES(...) _XMM_APPLY_I(_XMM_PAR_ARG_PACK_STRUCT_ENTRY, _XMM_SEP_SEMICOLON, __VA_ARGS__)
#define _XMM_PAR_ARG_PACK_PARAM_LIST(...) _XMM_APPLY(_XMM_PAR_ARG_PACK_STRUCT_ENTRY, __VA_ARGS__)

#define _XMM_PAR_ARG_PACK_EXTRACT_ARG_I(TYPE_, UNAME) __xmm_pargs->UNAME
#define _XMM_PAR_ARG_PACK_EXTRACT_ARG(PACK) _XMM_PAR_ARG_PACK_EXTRACT_ARG_I PACK
#define _XMM_PAR_ARG_PACK_EXTRACT_PACK(...) _XMM_APPLY(_XMM_PAR_ARG_PACK_EXTRACT_ARG,  __VA_ARGS__)

#define DECLARE_JOB_I(FNAME, STRUCT_NAME, ARG_PACKS, ARG_TYPES, SSIZE) \
  void FNAME ARG_TYPES; \
  struct STRUCT_NAME { \
    _XMM_PSHIM(_XMM_PAR_ARG_PACK_STRUCT_ENTRIES, ARG_PACKS) ; \
  }; \
  /* TODO: Factor out names! */ \
  static const size_t __xmm_gsu_swords_ ## FNAME = (SSIZE); \
  static void __xmm_ugs_shim_ ## FNAME  (void *__xmm_pargs_) \
  { \
    struct STRUCT_NAME * const __xmm_pargs = __xmm_pargs_; \
    FNAME(_XMM_PSHIM(_XMM_PAR_ARG_PACK_EXTRACT_PACK, ARG_PACKS)); \
    asm volatile("" : : : "memory"); \
  } /*\
  static void __xmm_ugs_dispatch_ ## FNAME (_XMM_PSHIM(_XMM_PAR_ARG_PACK_PARAM_LIST, ARG_PACKS)) \
  { \
    void *__xmm_pstack = memalign(4, SSIZE * 4);\
    struct STRUCT_NAME __xmm_pargs = {};\
    free(__xmm_pstack); \
  }*/

#define _XMM_JPAR_DISPATCH_II(FUNC, ARGS, SSIZE, ANAME, SNAME) \
  struct _XMM_PAR_ARG_STRUCT_NAME(FUNC) ANAME = {_XMM_UNPACK(ARGS)}; \
  char SNAME[_XMM_PAR_WSIZE * 128] __attribute__ ((aligned(_XMM_PAR_WSIZE))); \
  thread_group_add(__xmm_par_sync, (__xmm_ugs_shim_ ## FUNC), &ANAME, SNAME + _XMM_PAR_WSIZE * (SSIZE - 1));

#define _XMM_JPAR_DISPATCH_I(FUNC, ARG, ANAME, SNAME) _XMM_JPAR_DISPATCH_II(FUNC, ARG, (__xmm_gsu_swords_ ## FUNC), ANAME, SNAME)
#define _XMM_JPAR_DISPATCH(PACK) _XMM_PSHIM(_XMM_JPAR_DISPATCH_I, PACK, _XMM_UNIQUE_LABEL(__xmm_pargs), _XMM_UNIQUE_LABEL(__xmm_substack))

#define _XMM_JPAR_SYNCEXEC_I(FUNC, ARG_PACK) do { (FUNC) ARG_PACK; } while (0)
#define _XMM_JPAR_SYNCEXEC(PACK) _XMM_PSHIM(_XMM_JPAR_SYNCEXEC_I, PACK)

#define _XMM_JPAR_JOBS_I(FIRST, ...) \
  do { \
    const threadgroup_t __xmm_par_sync = __xcore_c_allocate_thread_group(); \
    _XMM_APPLY_NOSEP(_XMM_JPAR_DISPATCH, __VA_ARGS__) \
    thread_group_start(__xmm_par_sync); \
    _XMM_JPAR_SYNCEXEC(FIRST); \
    thread_group_wait_and_free(__xmm_par_sync); \
  } while (0)
