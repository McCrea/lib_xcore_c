// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_hwtimer_impl_h__
#define __xcore_c_hwtimer_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.


#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_resource_impl.h>
#include <xs1.h>

_XCORE_C_EXFUN
inline void _hwtimer_realloc_xc_timer(void)
{
  // __init_threadlocal_timer has resource ID in r2 and it may be zero.
  // Implement a checked version here instead.
  unsigned tmr, addr;
  _RESOURCE_ALLOC(tmr, XS1_RES_TYPE_TIMER);
  asm volatile( "ecallf %0" :: "r" (tmr));
  asm volatile( "ldaw %0, dp[__timers]" : "=r" (addr));
  asm volatile( "get r11, id" ::: /* clobbers */ "r11");
  asm volatile( "stw  %0, %1[r11]" : : "r" (tmr), "r" (addr));
}

extern void __free_threadlocal_timer(void);
_XCORE_C_EXFUN
inline void _hwtimer_free_xc_timer(void)
{
  __free_threadlocal_timer();
}

_XCORE_C_EXFUN
inline resource_t _hwtimer_alloc(void)
{
  resource_t t;
  _RESOURCE_ALLOC(t, XS1_RES_TYPE_TIMER);
  return t;
}

_XCORE_C_EXFUN
inline void _hwtimer_free(resource_t t)
{
  _resource_free(t);
}

_XCORE_C_EXFUN
inline uint32_t _hwtimer_get_time(resource_t t)
{
  register uint32_t now;
  asm volatile("in %0, res[%1]" : "=r" (now): "r" (t));
  return now;
}

_XCORE_C_EXFUN
inline uint32_t _hwtimer_get_trigger_time(resource_t t)
{
  uint32_t tval;
  asm volatile ("getd %0, res[%1]" : "=r" (tval) : "r" (t));
  return tval;
}

_XCORE_C_EXFUN
inline void _hwtimer_change_trigger_time(resource_t t, uint32_t time)
{
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

_XCORE_C_EXFUN
inline void _hwtimer_set_trigger_time(resource_t t, uint32_t time)
{
  _RESOURCE_SETCI(t, XS1_SETC_COND_AFTER);
  _hwtimer_change_trigger_time(t, time);
}

_XCORE_C_EXFUN
inline void _hwtimer_clear_trigger_time(resource_t t)
{
  _RESOURCE_SETCI(t, XS1_SETC_COND_NONE);
  // hwtimer_get_time() will respond immediately
}

#endif // !defined(__XC__)

#endif // __xcore_c_hwtimer_impl_h__
