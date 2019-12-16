// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_lock_h__
#define __xcore_c_lock_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <xcore/_support/xcore_c_lock_impl.h>
#include <xcore/_support/xcore_c_exception_impl.h>

/** Allocates a lock.
 *
 *  This function allocates a hardware lock.
 *  If there are no locks availble the function returns 0.
 *  When the lock is no longer required, lock_free() must be called
 *  to deallocate it.
 *
 *  \param l    lock_t variable representing the initialised lock
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *\*l* argument.
 */
inline lock_t lock_alloc()
{
  return _lock_alloc();
}

/** Deallocate a lock.
 *
 *  This function frees the hardware lock.
 *  The lock must be released prior to calling this function.
 *
 *  \param l    The lock_t to be freed
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated lock,
 *                                    or the lock has not been released.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the lock.
 *  \exception  ET_LOAD_STORE         invalid *\*l* argument.
 */
inline void lock_free(lock_t l)
{
  _resource_free((resource_t)l);
}

/** Acquire a lock.
 *
 *  Only one core at a time can acquire a lock. This provides a hardware mutex
 *  which have very low overheads. If another thread has already acquired this
 *  lock then this function will pause until the lock is released and this core
 *  becomes the owner.
 *
 *  \param l    The lock_t to acquire
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated lock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the lock.
 */
inline void lock_acquire(lock_t l)
{
  _lock_acquire(l);
}

/** Release a lock.
 *
 *  This releases the lock and allocates the next owner from the list of waiting
 *  cores in round-robin manner.
 *  *Note*: there are no checks that the core releasing the lock is the current
 *  owner.
 *
 *  \param l    The lock_t to use release
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated lock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the lock.
 */
inline void lock_release(lock_t l)
{
  _lock_release(l);
}

#endif // !defined(__XC__)

#endif // __xcore_c_lock_h__
