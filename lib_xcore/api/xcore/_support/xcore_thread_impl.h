// Copyright (c) 2019-2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_resource_impl.h>

typedef resource_t __xcore_thread_t;

void __xcore_synchronised_thread_end(void);
void __xcore_unsynchronised_thread_end(void);

_XCORE_EXFUN
inline resource_t __xcore_allocate_thread_group(void)
{
  resource_t res;
  asm volatile("getr %[res], %[type]" 
               : [res] "=r" (res) 
               : [type] "n" (0x3));
  return res;
}

_XCORE_EXFUN
inline __xcore_thread_t __xcore_create_synchronised_thread(const resource_t sync)
{
  __xcore_thread_t thread;
  asm volatile("getst %[thread], res[%[sync]]" : [thread] "=r" (thread) : [sync] "r" (sync));
  return thread;
}

_XCORE_EXFUN
inline void __xcore_set_thread_worker(const __xcore_thread_t thread, (* const func)(void *))
{
  asm volatile("init t[%[thread]]:pc, %[new_pc]" : : [thread] "r" (thread), [new_pc] "r" (func));
}

_XCORE_EXFUN
inline void __xcore_set_thread_stack(const __xcore_thread_t thread, void * const stack_base)
{
  asm volatile("init t[%[thread]]:sp, %[new_sp]" : : [thread] "r" (thread), [new_sp] "r" (stack_base));
}

_XCORE_EXFUN
inline void __xcore_set_thread_parameter0(const __xcore_thread_t thread, void * const parameter)
{
  asm volatile("set t[%[thread]]:r0, %[arg]" : : [thread] "r" (thread), [arg] "r" (parameter));
}

_XCORE_EXFUN
inline void __xcore_set_thread_terminator(const __xcore_thread_t thread, void(* const terminator)(void))
{
  asm volatile("init t[%[thread]]:lr, %[new_lr]" : : [thread] "r" (thread), [new_lr] "r" (terminator));
}

_XCORE_EXFUN
inline __xcore_thread_t __xcore_allocate_unsynchronised_thread(void)
{
  __xcore_thread_t res;
  asm volatile("getr %[res], %[type]" 
               : [res] "=r" (res) 
               : [type] "n" (0x4));
  return res;
}

_XCORE_EXFUN
inline void __xcore_thread_group_sync(const resource_t group)
{
  asm volatile("msync res[%[sync]]" 
               : /* No outputs */
               : [sync] "r" (group) : "memory"); // Other threads may use our variables.
}

_XCORE_EXFUN
inline void __xcore_thread_group_join(const resource_t group)
{
  asm volatile("mjoin res[%[sync]]" 
               : /* No outputs */
               : [sync] "r" (group) 
               : "memory"); // Other threads may have clobbered our variables.
}

_XCORE_EXFUN
inline void __xcore_unsynronised_thread_start(const __xcore_thread_t thread)
{
  asm volatile("start t[%[thread]]" 
               : /* No outputs */
               : [thread] "r" (thread) : "memory"); // Other thread may use our variables.
}

