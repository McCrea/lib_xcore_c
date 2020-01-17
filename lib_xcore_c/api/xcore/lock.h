// Copyright (c) 2016, XMOS Ltd, All rights reserved

/** \file
 * \brief provides hardware-based locking
 */

#ifndef __xcore_c_lock_h__
#define __xcore_c_lock_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <xcore/_support/xcore_c_lock_impl.h>

/** \brief Allocates a lock.
 *
 *  If there are no locks availble the function returns 0.
 *  \note When the lock is no longer required, lock_free() must be called
 *        to deallocate it.
 *
 *  \return  handle for the allocated lock, 0 is no locks were available
 */
inline lock_t lock_alloc()
{
  return _lock_alloc();
}

/** \brief Deallocates a given lock.
 *
 *  The lock must be released prior to calling this function.
 *
 *  \param l  The lock_t to be freed
 *
 *  \exception  ET_ILLEGAL_RESOURCE  not an allocated lock,
 *                                   or the lock has not been released.
 *  \exception  ET_RESOURCE_DEP      another core is actively changing the lock.
 *  \exception  ET_LOAD_STORE        invalid \a l argument.
 */
inline void lock_free(lock_t l)
{
  _resource_free((resource_t)l);
}

/** \brief Acquire a lock.
 *
 *  Only one core at a time can acquire a lock. This provides a hardware mutex
 *  which have very low overheads. If another thread has already acquired this
 *  lock then this function will pause until the lock is released and this core
 *  becomes the owner.
 *
 *  \param l  The lock_t to acquire
 *
 *  \exception  ET_ILLEGAL_RESOURCE  not an allocated lock.
 *  \exception  ET_RESOURCE_DEP      another core is actively changing the lock.
 */
inline void lock_acquire(lock_t l)
{
  _lock_acquire(l);
}

/** \brief Release a lock.
 *
 *  Releases the lock and allocates the next owner from the list of waiting
 *  cores in round-robin manner.
 *  \note There are no checks that the core releasing the lock is the current
 *        owner.
 *
 *  \param l  The lock_t to use release
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
