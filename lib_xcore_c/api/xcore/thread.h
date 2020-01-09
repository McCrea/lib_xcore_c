#pragma once

#include <xcore/_support/xcore_c_thread_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>

inline threadgroup_t thread_group_alloc(void)
{
  return __xcore_c_allocate_thread_group();
}

inline void thread_group_add(const threadgroup_t group, 
                             const thread_function_t func, 
                             void * const argument, 
                             void * const stack_base)
{
  const _xcore_thread_t thread = __xcore_c_create_synchronised_thread(group);
  __xcore_c_set_thread_worker(thread, func);
  __xcore_c_set_thread_stack(thread, stack_base);
  __xcore_c_set_thread_parameter0(thread, argument);
  __xcore_c_set_thread_terminator(thread, _xcore_c_synchronised_thread_end);
}

inline void thread_group_start(const threadgroup_t group)
{
  __xcore_c_thread_group_sync(group);
}

inline void thread_group_free(const threadgroup_t group)
{
  _resource_free((resource_t)group);
}

inline void thread_group_wait(const threadgroup_t group)
{
  __xcore_c_thread_group_join(group);
}

inline void thread_group_wait_and_free(const threadgroup_t group)
{
  thread_group_wait(group);
  thread_group_free(group);
}

inline xthread_t xthread_alloc_and_start(const thread_function_t func, 
                                         void * const argument,
                                         void * const stack_base)
{
  const _xcore_thread_t group = thread_group_alloc();
  thread_group_add(group, func, argument, stack_base);
  thread_group_start(group);
  return group;
}

inline void xthread_wait_and_free(const xthread_t thread)
{
  thread_group_wait_and_free(thread);
}

inline void run_async(const thread_function_t func,
                      void * const argument,
                      void * const stack_base)
{
  const _xcore_thread_t thread = __xcore_c_allocate_unsynchronised_thread();
  __xcore_c_set_thread_worker(thread, func);
  __xcore_c_set_thread_stack(thread, stack_base);
  __xcore_c_set_thread_parameter0(thread, argument);
  __xcore_c_set_thread_terminator(thread, _xcore_c_unsynchronised_thread_end);
  __xcore_c_unsynronised_thread_start(thread);
}
