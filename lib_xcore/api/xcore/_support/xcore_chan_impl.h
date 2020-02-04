// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.
#include <stdint.h>
#include <xs1.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_resource_impl.h>
#include <xcore/_support/xcore_meta_macro.h>

typedef resource_t __xcore_streaming_chanend_t;

_XCORE_EXFUN
inline __xcore_streaming_chanend_t __xcore_chanend_alloc(void)
{
  __xcore_streaming_chanend_t c;
  _RESOURCE_ALLOC(c, XS1_RES_TYPE_CHANEND);
  return c;
}

_XCORE_EXFUN
inline void __xcore_chanend_free(resource_t c)
{
  __xcore_resource_free(c);
}

_XCORE_EXFUN
inline void __xcore_chanend_set_dest(resource_t c, resource_t dst)
{
  asm("setd res[%0], %1" :: "r" (c), "r" (dst));
}

_XCORE_EXFUN
inline resource_t __xcore_chanend_get_dest(resource_t c)
{
  uint32_t data;
  asm volatile("getd %[dest], res[%[chanend]]" : [dest] "=r" (data) : [chanend] "r" (c));
  return data;
}

_XCORE_EXFUN
inline void __xcore_chanend_out_word(resource_t c, uint32_t data)
{
  asm volatile("out res[%0], %1" :: "r" (c), "r" (data));
}

_XCORE_EXFUN
inline void __xcore_chanend_out_byte(resource_t c, uint8_t data)
{
  asm volatile("outt res[%0], %1" :: "r" (c), "r" (data));
}

_XCORE_EXFUN
inline uint32_t __xcore_chanend_in_word(resource_t c)
{
  uint32_t data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

_XCORE_EXFUN
inline uint8_t __xcore_chanend_in_byte(resource_t c)
{
  uint8_t data;
  asm volatile("int %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

_XCORE_EXFUN
inline uint8_t __xcore_chanend_in_ct(resource_t c)
{
  uint8_t data;
  asm volatile("inct %[token], res[%[chanend]]" : [token] "=r" (data) : [chanend] "r" (c));
  return data;
}

#define _XCORE_CHANEND_OUTCTI_CASE_N_ASM(N) \
  case N: \
    asm volatile("outct res[%[chanend]], %[token]" :: [chanend] "r" (c), [token] "n" (N)); \
    break;

_XCORE_EXFUN
inline void __xcore_chanend_out_ct(resource_t c, uint8_t ct)
{
  if (__builtin_constant_p(ct) && ct <= 11)
  {
    switch (ct)
    {
      _XCORE_APPLY_NOSEP(_XCORE_CHANEND_OUTCTI_CASE_N_ASM, 0,1,2,3,4,5,6,7,8,9,10,11)
      default: __builtin_unreachable(); break;
    }
  }
  else
  {
    asm volatile("outct res[%0], %1" :: "r" (c), "r" (ct));
  }
}

#define _XCORE_CHANEND_CHKCTI_CASE_N_ASM(N) \
  case N: \
    asm volatile("chkct res[%[chanend]], %[token]" :: [chanend] "r" (c), [token] "n" (N)); \
    break;

_XCORE_EXFUN
inline void __xcore_chanend_check_ct(resource_t c, const uint8_t ct)
{
  if (__builtin_constant_p(ct) && ct <= 11)
  {
    switch (ct)
    {
      _XCORE_APPLY_NOSEP(_XCORE_CHANEND_CHKCTI_CASE_N_ASM, 0,1,2,3,4,5,6,7,8,9,10,11)
      default: __builtin_unreachable(); break;
    }
  }
  else
  {
    asm volatile("chkct res[%0], %1" :: "r" (c), "r" (ct));
  }
}

_XCORE_EXFUN
inline _Bool __xcore_chanend_test_control_token(resource_t c)
{
  unsigned res;
  asm volatile("testct %[res], res[%[chanend]]" : [res] "=r" (res) : [chanend] "r" (c));
  return res;
}


_XCORE_EXFUN
inline unsigned __xcore_chanend_test_control_token_word(resource_t c)
{
  unsigned res;
  asm volatile("testwct %[res], res[%[chanend]]" : [res] "=r" (res) : [chanend] "r" (c));
  return res;
}

_XCORE_EXFUN
inline _Bool __xcore_chanend_test_dest_local(resource_t c)
{
  unsigned res;
  asm volatile("testlcl %[res], res[%[chanend]]" : [res] "=r" (res) : [chanend] "r" (c));
  return res;
}

_XCORE_EXFUN
inline void __xcore_channend_set_network(resource_t c, unsigned n)
{
  asm volatile("setn res[%[chanend]], %[net]" :: [chanend] "r" (c), [net] "r" (n));
}

_XCORE_EXFUN
inline unsigned __xcore_channend_get_network(resource_t c)
{
  unsigned net;
  asm volatile("getn %[net], res[%[chanend]]" : [net] "=r" (net) : [chanend] "r" (c));
  return net;
}
