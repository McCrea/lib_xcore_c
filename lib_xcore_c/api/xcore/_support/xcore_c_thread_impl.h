#pragma once

#include <xcore/_support/xcore_c_resource_impl.h>

typedef resource_t xthread_t;
typedef resource_t threadgroup_t;
typedef resource_t _xcore_thread_t;
typedef void (*thread_function_t)(void *);

void _xcore_c_synchronised_thread_end(void);
void _xcore_c_unsynchronised_thread_end(void);

inline threadgroup_t __xcore_c_allocate_thread_group(void)
{
  threadgroup_t res;
  asm volatile("getr %[res], %[type]" 
               : [res] "=r" (res) 
               : [type] "n" (0x3));
  return res;
}

inline _xcore_thread_t __xcore_c_create_synchronised_thread(const threadgroup_t sync)
{
  _xcore_thread_t thread;
  asm volatile("getst %[thread], res[%[sync]]" : [thread] "=r" (thread) : [sync] "r" (sync));
  return thread;
}

inline void __xcore_c_set_thread_worker(const _xcore_thread_t thread, const thread_function_t func)
{
  asm volatile("init t[%[thread]]:pc, %[new_pc]" : : [thread] "r" (thread), [new_pc] "r" (func));
}

inline void __xcore_c_set_thread_stack(const _xcore_thread_t thread, void * const stack_base)
{
  asm volatile("init t[%[thread]]:sp, %[new_sp]" : : [thread] "r" (thread), [new_sp] "r" (stack_base));
}

inline void __xcore_c_set_thread_parameter0(const _xcore_thread_t thread, void * const parameter)
{
  asm volatile("set t[%[thread]]:r0, %[arg]" : : [thread] "r" (thread), [arg] "r" (parameter));
}

inline void __xcore_c_set_thread_terminator(const _xcore_thread_t thread, void(* const terminator)(void))
{
  asm volatile("init t[%[thread]]:lr, %[new_lr]" : : [thread] "r" (thread), [new_lr] "r" (terminator));
}

inline _xcore_thread_t __xcore_c_allocate_unsynchronised_thread(void)
{
  _xcore_thread_t res;
  asm volatile("getr %[res], %[type]" 
               : [res] "=r" (res) 
               : [type] "n" (0x4));
  return res;
}

inline void __xcore_c_thread_group_sync(const threadgroup_t group)
{
  asm volatile("msync res[%[sync]]" 
               : /* No outputs */
               : [sync] "r" (group) : "memory"); // Other threads may use our variables.
}

inline void __xcore_c_thread_group_join(const threadgroup_t group)
{
  asm volatile("mjoin res[%[sync]]" 
               : /* No outputs */
               : [sync] "r" (group) 
               : "memory"); // Other threads may have clobbered our variables.
}

inline void __xcore_c_unsynronised_thread_start(const _xcore_thread_t thread)
{
  asm volatile("start t[%[thread]]" 
               : /* No outputs */
               : [thread] "r" (thread) : "memory"); // Other thread may use our variables.
}

