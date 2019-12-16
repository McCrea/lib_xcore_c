// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_clock_impl_h__
#define __xcore_c_clock_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <xccompat.h>
#ifdef __DOXYGEN__
// Copy typedefs from xccompat.h for use by doxygen
/**
 * clock is an opaque type for use in C/C++ code.
 * It enables a xC function prototyped as taking a parameter of type clock to
 * be called from C and vice versa.
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned xclock;
#else //TODO - remove this once naming sorted
typedef unsigned xclock;
#endif

inline void _clock_set_ready_src(xclock clk, port ready_source)
{
  asm volatile("setrdy res[%0], %1" :: "r" (clk), "r" (ready_source));
}

inline void _clock_set_source_port(xclock clk, port p)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (p));
}

inline void _clock_set_source_clk_ref(xclock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (XS1_CLK_REF));
}

inline void _clock_set_source_clk_xcore(xclock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (XS1_CLK_XCORE));
}

inline void _clock_set_divide(xclock clk, uint8_t divide)
{
  asm volatile("setd res[%0], %1" :: "r" (clk), "r" (divide));
}

#endif // !defined(__XC__)

#endif // __xcore_c_clock_impl_h__
