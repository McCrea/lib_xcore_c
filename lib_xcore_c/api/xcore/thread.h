#pragma once

/** \file
 *  \brief Hardware-assisted threading support
 */

#include <xcore/_support/xcore_c_thread_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>

/** \brief Allocates a hardware thread
 *
 *  Attempts to allocate a thread group from the pool on the current tile.
 *  \note The thread group should be freed using thread_group_free() when it is no longer
 *        required. (Or freed as a consequence of calling thread_group_wait_and_free().)
 * 
 *  \return A thread group handle, or 0 if none were available.
 */
inline threadgroup_t thread_group_alloc(void)
{
  return __xcore_c_allocate_thread_group();
}

/** \brief Add a new task to a thread group.
 *
 *  Adds a thead function invocation to a thread group allocated using thread_group_alloc().
 *  This configures a hardware thread to execute \a func with \a argument as its sole parameter
 *  and with its stack pointer initialised to \a stack_base.
 *  \a stack_base must be word aligned and point to the last word of a block of memory sufficient 
 *  to satisfy <tt>func</tt>'s stack requirements.
 *  That is, for a stack requirement of \c s words, <tt>[stack_base-s*word_size, stack_base)</tt> 
 *  will be  used as the thread's stack and will be clobbered.
 *  
 *  \param group           Thread group handle as returned by thread_group_alloc().
 *  \param func            Function to call in separate thread with siganture <tt>void(void*)</tt>.
 *  \param[in] argument    Parameter to pass to \a func.
 *  \param[in] stack_base  Word aligned pointer to the last word of the region to use as a stack 
 *                         when calling \a func. Note that this can be calculated with stack_base().
 *
 * \note Execution of \a func will not begin until the group is started using thread_group_start().
 */
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

/** \brief Starts all threads in a group running.
 *
 *  Starts execution of the thread functions for each thread in the group (as added using thread_group_add()).
 *  This function will return immediately regardless of the state of the threads.
 *  \note Use thread_group_wait() or thread_group_wait_and_free() to wait for the thread group to finish.
 *
 *  \param group  The thread group to start.
 */
inline void thread_group_start(const threadgroup_t group)
{
  __xcore_c_thread_group_sync(group);
}

/** \brief Frees a thread group.
 * 
 *  Returns the thread group to the pool so that it may be allocated again. The group handle
 *  is invalid once passed to this function so must not be re-used.
 *  \attention This function must not be called on a thread group which has been started
 *             but not waited upon, even if its constituent threads have finished executing.
 *
 *  \param group  The group to free. 
 */
inline void thread_group_free(const threadgroup_t group)
{
  _resource_free((resource_t)group);
}

/** \brief Wait for all threads in a thread group to finish.
 *
 *  The group must have been started using thread_group_start().
 *  Calls to this function will block until all threads in the group have finished executing.
 *  Upon return of this function, the \a group remains valid but will no longer have any threads
 *  associated with it. The group may be re-used but threads must be re-added to it.
 *
 *  \note Since the group remains valid, it should be freed with thread_group_free() if no longer required.
 *
 *  \param group  The group to wait for completion.
 */
inline void thread_group_wait(const threadgroup_t group)
{
  __xcore_c_thread_group_join(group);
}

/** \brief Waits for a thread group to finish then frees it.
 *  
 *  Helper to both wait for and free a thread group. The group must have been started using
 *  thread_group_start() and the group will be invalid once this function returns.
 *
 * \b Calls \li thread_group_wait() \li thread_group_free()
 */
inline void thread_group_wait_and_free(const threadgroup_t group)
{
  thread_group_wait(group);
  thread_group_free(group);
}

/** \brief Runs a function in another thread and returns a waitable handle.
 *
 *  Starts executing \a func in a separate hardware thread with \a argument as its sole parameter
 *  and with its stack pointer initialised to \a stack_base.
 *  \a stack_base must be word aligned and point to the last word of a block of memory sufficient
 *  to satisfy <tt>func</tt>'s stack requirements.
 *  That is, for a stack requirement of \c s words, <tt>[stack_base-s*word_size, stack_base)</tt> 
 *  will be  used as the thread's stack and will be clobbered.
 *
 *  \note The thread will begin execution immediately and this function will return.
 *        The thread will not be returned to the pool upon completion - it is necessary to call 
 *        xthread_wait_and_free() to free the thread so that it may be reused.
 *
 *  \param func            Function to call in separate thread with siganture <tt>void(void*)</tt>.
 *  \param[in] argument    Parameter to pass to \a func.
 *  \param[in] stack_base  Word aligned pointer to the last word of the region to use as a stack 
 *                         when calling \a func. Note that this can be calculated with stack_base().
 *  \return A waitable handle for the hardware thread.
 */
inline xthread_t xthread_alloc_and_start(const thread_function_t func, 
                                         void * const argument,
                                         void * const stack_base)
{
  const _xcore_thread_t group = thread_group_alloc();
  thread_group_add(group, func, argument, stack_base);
  thread_group_start(group);
  return group;
}

/** \brief Wait for a thread to finish and then free it.
 * 
 *  \a thread must have been allocated and started using xthread_alloc_and_start().
 *  Calls to this function will block until the function called by \a thread has returned
 *  and upon completion it will free the associated hardware thread. \a thread must not be re-used
 *  after it has been freed.
 *
 *  \param thread  The thread to wait on, as returned by xthread_alloc_and_start().
 */
inline void xthread_wait_and_free(const xthread_t thread)
{
  thread_group_wait_and_free(thread);
}


/** \brief Runs a function in another hardware thread.
 *
 *  Starts executing \a func in a separate hardware thread with \a argument as its sole parameter
 *  and with its stack pointer initialised to \a stack_base.
 *  \a stack_base must be word aligned and point to the last word of a block of memory sufficient
 *  to satisfy <tt>func</tt>'s stack requirements.
 *  That is, for a stack requirement of \c s words, <tt>[stack_base-s*word_size, stack_base)</tt> 
 *  will be  used as the thread's stack and will be clobbered.
 *
 *  \note The associated hardware thread will be freed once \a func returns, but it is not trivially
 *        possible to determine wether or not this has happened from the calling thread.
 *        If it necessary to wait for the completion of \a func then xthread_alloc_and_start() and
 *        xthread_wait_and_free() should be used instead.
 *
 *  \param func            Function to call in separate thread with siganture <tt>void(void*)</tt>.
 *  \param[in] argument    Parameter to pass to \a func.
 *  \param[in] stack_base  Word aligned pointer to the last word of the region to use as a stack 
 *                         when calling \a func. Note that this can be calculated with stack_base().
 */
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


/** \brief Returns a stack pointer suitable for use as a \c stack_base argument 
 *         given a base address and a size.
 * 
 *   Given a base pointer (e.g. as returned by malloc or found by taking the address of an object)
 *   and the size of the intended stack in words, returns a stack base pointer to the
 *   last word of the stack - which is suitable for passing to the \c stack_base arguments of
 *   xthread_alloc_and_start(), run_async() and thread_group_add(). The given base address
 *   must be word aligned as the resulting stack pointer is required to be word aligned also.
 *   The resulting pointer will be a valid stack pointer for a stack \p words words in size.
 *   If used as a stack pointer for a function with a stack requirement no greater than \p words
 *   words then the memory region used as a stack by that function will not be beyond
 *   <tt>[mem_base, (char *)mem_base + words*WORD_SIZE)</tt> in either direction.
 *
 *   \param[in] mem_base  The base (lowest) address of the object/region to use as a stack.
 *                        Must be word aligned.
 *   \param words         Size of the stack the returned pointed will return to in words
 *   \return              The stack pointer.
 */
inline void *stack_base(void * const mem_base, size_t const words)
{
  return (void *)((char *)mem_base + sizeof(int)*(words-1));
}
