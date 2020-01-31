// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <limits.h>
#include <stdint.h>
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_reference_time.h>

_XCORE_EXFUN
inline void __xcore_ecallt(int value)
{
  if (!(__builtin_constant_p(value) && !value))
  {
    asm volatile("ecallt %[value]" : : [value] "r" (value));
  }
  __builtin_assume(value == 0);
}

_XCORE_EXFUN
inline void __xcore_ecallf(int value)
{
  if (!(__builtin_constant_p(value) && value))
  {
    asm volatile("ecallf %[value]" : : [value] "r" (value));
  }
  __builtin_assume(value != 0);
}

inline _Bool __xcore_not_after_reference_time(uint32_t v)
{
    const uint32_t r = __xcore_get_reference_time();
    const unsigned tolerance = 1 << ((sizeof(int)*CHAR_BIT)-1);
    return v >= tolerance
      ? (r <= v || r > v+tolerance)
      : (r <= v && r > v-tolerance); 
}

_XCORE_EXFUN
inline void __xcore_elate(uint32_t value)
{
#ifdef _XCORE_HAS_REFERENCE_CLOCK
  asm volatile("elate %[value]" : : [value] "r" (value));
#else
  __xcore_ecallf(__xcore_not_after_reference_time(value));
#endif
}

