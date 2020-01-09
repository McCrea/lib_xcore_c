#pragma once

#include <xcore/_support/xcore_c_meta_macro.h>
#include <xcore/thread.h>

#define _XMM_PAR_WSIZE 4

#define _XMM_PAR_PRIME_II(FUNC, ARG, SSIZE, SNAME) \
  volatile char SNAME[_XMM_PAR_WSIZE * SSIZE] __attribute__ ((aligned(_XMM_PAR_WSIZE))); \
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
