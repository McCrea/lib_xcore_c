// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 * \brief provides hardware-based locking
 */

#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_lock_impl.h>

/** \brief Hardware lock handle type. */
typedef resource_t lock_t;


/** \brief Allocates a lock.
 *
 *  If there are no locks availble the function returns 0.
 *  \note When the lock is no longer required, lock_free() must be called
 *        to deallocate it.
 *
 *  \return  handle for the allocated lock, 0 is no locks were available
 */
_XCORE_EXFUN
inline lock_t lock_alloc()
{
  return __xcore_lock_alloc();
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
_XCORE_EXFUN
inline void lock_free(lock_t l)
{
  __xcore_resource_free((resource_t)l);
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
_XCORE_EXFUN
inline void lock_acquire(lock_t l)
{
  __xcore_lock_acquire(l);
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
_XCORE_EXFUN
inline void lock_release(lock_t l)
{
  __xcore_lock_release(l);
}

