// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_c_common.h>

_XCORE_C_EXFUN
inline void __xcore_c_ecallt(unsigned value)
{
  if (!(__builtin_constant_p(value) && !value))
  {
    asm volatile("ecallt %[value]" : : [value] "r" (value));
  }
  __builtin_assume(value == 0);
}

_XCORE_C_EXFUN
inline void __xcore_c_ecallf(unsigned value)
{
  if (!(__builtin_constant_p(value) && value))
  {
    asm volatile("ecallf %[value]" : : [value] "r" (value));
  }
  __builtin_assume(value != 0);
}

_XCORE_C_EXFUN
inline void __xcore_c_elate(unsigned value)
{
#if !defined(__XS1B__) && !defined(__XS1C__)
  asm volatile("elate %[value]" : : [value] "r" (value));
#else
  __xcore_c_ecallf(0); //TODO 
#endif
}
