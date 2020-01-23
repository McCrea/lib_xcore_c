// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_lock_impl_h__
#define __xcore_c_lock_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_resource_impl.h>
#include <xs1.h>

_XCORE_C_EXFUN
inline resource_t _lock_alloc(void)
{
  resource_t l;
  _RESOURCE_ALLOC(l, XS1_RES_TYPE_LOCK);
  return l;
}

_XCORE_C_EXFUN
inline void _lock_acquire(resource_t l)
{
  unsigned dummy;
  asm volatile("in %0, res[%1]" : "=r" (dummy): "r" (l) : "memory");
}

_XCORE_C_EXFUN
inline void _lock_release(resource_t l)
{
  asm volatile("out res[%0], %0" :: "r" (l) : "memory");
}

#endif // !defined(__XC__)

#endif // __xcore_c_lock_impl_h__
