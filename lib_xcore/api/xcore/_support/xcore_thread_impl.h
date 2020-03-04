// Copyright (c) 2019-2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_resource_impl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef resource_t __xcore_thread_t;

void __xcore_synchronised_thread_end(void);
void __xcore_unsynchronised_thread_end(void);

_XCORE_EXFUN
inline resource_t __xcore_allocate_thread_group(void) _XCORE_NOTHROW
{
  resource_t __res;
  asm volatile("getr %[__res], %[__type]" 
               : [__res] "=r" (__res) 
               : [__type] "n" (0x3));
  return __res;
}

_XCORE_EXFUN
inline __xcore_thread_t __xcore_create_synchronised_thread(const resource_t __sync) _XCORE_NOTHROW
{
  __xcore_thread_t __xthread;
  asm volatile("getst %[__xthread], res[%[__sync]]"
               : [__xthread] "=r" (__xthread)
               : [__sync] "r" (__sync));
  return __xthread;
}

_XCORE_EXFUN
inline void __xcore_set_thread_worker(const __xcore_thread_t __xthread,
  void(* const __func)(void *)) _XCORE_NOTHROW
{
  asm volatile("init t[%[__xthread]]:pc, %[__new_pc]"
               : /* No outputs */
               : [__xthread] "r" (__xthread)
               , [__new_pc] "r" (__func));
}

_XCORE_EXFUN
inline void __xcore_set_thread_stack(const __xcore_thread_t __xthread,
                                     void * const __stack_base) _XCORE_NOTHROW
{
  asm volatile("init t[%[__xthread]]:sp, %[__new_sp]"
               : /* No outputs */
               : [__xthread] "r" (__xthread)
               , [__new_sp] "r" (__stack_base));
}

_XCORE_EXFUN
inline void __xcore_set_thread_parameter0(const __xcore_thread_t __xthread,
                                          void * const __parameter) _XCORE_NOTHROW
{
  asm volatile("set t[%[__xthread]]:r0, %[__arg]"
               : /* No outputs */
               : [__xthread] "r" (__xthread)
               , [__arg] "r" (__parameter));
}

_XCORE_EXFUN
inline void __xcore_set_thread_terminator(const __xcore_thread_t __xthread,
                                          void(* const __terminator)(void)) _XCORE_NOTHROW
{
  asm volatile("init t[%[__xthread]]:lr, %[__new_lr]"
               : /* No outputs */
               : [__xthread] "r" (__xthread)
               , [__new_lr] "r" (__terminator));
}

_XCORE_EXFUN
inline __xcore_thread_t __xcore_allocate_unsynchronised_thread(void) _XCORE_NOTHROW
{
  __xcore_thread_t __res;
  asm volatile("getr %[__res], %[__type]" 
               : [__res] "=r" (__res) 
               : [__type] "n" (0x4));
  return __res;
}

_XCORE_EXFUN
inline void __xcore_thread_group_sync(const resource_t __group) _XCORE_NOTHROW
{
  asm volatile("msync res[%[__sync]]" 
               : /* No outputs */
               : [__sync] "r" (__group)
               : "memory"); // Other threads may use our variables.
}

_XCORE_EXFUN
inline void __xcore_thread_group_join(const resource_t __group) _XCORE_NOTHROW
{
  asm volatile("mjoin res[%[__sync]]" 
               : /* No outputs */
               : [__sync] "r" (__group) 
               : "memory"); // Other threads may have clobbered our variables.
}

_XCORE_EXFUN
inline void __xcore_unsynronised_thread_start(const __xcore_thread_t __xthread) _XCORE_NOTHROW
{
  asm volatile("start t[%[__xthread]]" 
               : /* No outputs */
               : [__xthread] "r" (__xthread)
               : "memory"); // Other thread may use our variables.
}

#ifdef __cplusplus
}
#endif

