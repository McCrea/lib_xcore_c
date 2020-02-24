// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once
// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <xcore/_support/xcore_common.h>

#ifdef __cplusplus
extern "C" {
#endif


_XCORE_EXFUN
inline void __xcore_clock_set_ready_src(resource_t clk, resource_t ready_source)
{
  asm volatile("setrdy res[%0], %1" :: "r" (clk), "r" (ready_source));
}

_XCORE_EXFUN
inline void __xcore_clock_set_source_port(resource_t clk, resource_t p)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (p));
}

_XCORE_EXFUN
inline void __xcore_clock_set_source_clk_ref(resource_t clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (XS1_CLK_REF));
}

_XCORE_EXFUN
inline void __xcore_clock_set_source_clk_xcore(resource_t clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (XS1_CLK_XCORE));
}

_XCORE_EXFUN
inline void __xcore_clock_set_divide(resource_t clk, uint8_t divide)
{
  asm volatile("setd res[%0], %1" :: "r" (clk), "r" (divide));
}

#ifdef __cplusplus
}
#endif

