// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <stdint.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_feature_test_impl.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _XCORE_HAS_REFERENCE_CLOCK

_XCORE_EXFUN
inline uint32_t __xcore_get_reference_time(void)
{
  uint32_t time;
  asm volatile ("gettime %[reftime]" : [reftime] "=r" (time));
  return time;
}

#endif

#ifdef __cplusplus
}
#endif

