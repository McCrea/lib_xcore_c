// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.
#include <xs1.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_resource_impl.h>

_XCORE_EXFUN
inline resource_t __xcore_lock_alloc(void)
{
  resource_t l;
  _RESOURCE_ALLOC(l, XS1_RES_TYPE_LOCK);
  return l;
}

_XCORE_EXFUN
inline void __xcore_lock_acquire(resource_t l)
{
  unsigned dummy;
  asm volatile("in %0, res[%1]" : "=r" (dummy): "r" (l) : "memory");
}

_XCORE_EXFUN
inline void __xcore_lock_release(resource_t l)
{
  asm volatile("out res[%0], %0" :: "r" (l) : "memory");
}
