// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_c_common.h>

_XCORE_EXFUN
inline void __xcore_ecallt(unsigned value)
{
  if (!(__builtin_constant_p(value) && !value))
  {
    asm volatile("ecallt %[value]" : : [value] "r" (value));
  }
  __builtin_assume(value == 0);
}

_XCORE_EXFUN
inline void __xcore_ecallf(unsigned value)
{
  if (!(__builtin_constant_p(value) && value))
  {
    asm volatile("ecallf %[value]" : : [value] "r" (value));
  }
  __builtin_assume(value != 0);
}

_XCORE_EXFUN
inline void __xcore_elate(unsigned value)
{
#if !defined(__XS1B__) && !defined(__XS1C__)
  asm volatile("elate %[value]" : : [value] "r" (value));
#else
  __xcore_ecallf(0); //TODO 
#endif
}
