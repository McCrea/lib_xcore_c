// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.
#include <stdint.h>
#include <xs1.h>
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_resource_impl.h>

typedef resource_t __xcore_streaming_chanend_t;

/** An opaque type for handling transactions
 *
 *  Users must not access its raw underlying type.
 */
typedef struct transacting_chanend_t {
  __xcore_streaming_chanend_t c;
  unsigned last_out;
} transacting_chanend_t;

_XCORE_EXFUN
inline __xcore_streaming_chanend_t __xcore_s_chanend_alloc(void)
{
  __xcore_streaming_chanend_t c;
  _RESOURCE_ALLOC(c, XS1_RES_TYPE_CHANEND);
  return c;
}

_XCORE_EXFUN
inline void __xcore_s_chanend_free(resource_t c)
{
  __xcore_resource_free(c);
}

_XCORE_EXFUN
inline void __xcore_s_chanend_set_dest(resource_t c, resource_t dst)
{
  asm("setd res[%0], %1" :: "r" (c), "r" (dst));
}

_XCORE_EXFUN
inline void __xcore_s_chan_out_word(resource_t c, uint32_t data)
{
  asm volatile("out res[%0], %1" :: "r" (c), "r" (data));
}

_XCORE_EXFUN
inline void __xcore_s_chan_out_byte(resource_t c, uint8_t data)
{
  asm volatile("outt res[%0], %1" :: "r" (c), "r" (data));
}

_XCORE_EXFUN
inline uint32_t __xcore_s_chan_in_word(resource_t c)
{
  uint32_t data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

_XCORE_EXFUN
inline uint8_t __xcore_s_chan_in_byte(resource_t c)
{
  uint8_t data;
  asm volatile("int %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

_XCORE_EXFUN
inline void __xcore_s_chan_out_ct(resource_t c, uint8_t ct)
{
  asm volatile("outct res[%0], %1" :: "r" (c), "r" (ct));
}

_XCORE_EXFUN
inline void __xcore_s_chan_out_ct_end(resource_t c)
{
  asm volatile("outct res[%0], " _XCORE_STR(XS1_CT_END) :: "r" (c));
}

_XCORE_EXFUN
inline void __xcore_s_chan_check_ct(resource_t c, uint8_t ct)
{
  asm volatile("chkct res[%0], %1" :: "r" (c), "r" (ct));
}

_XCORE_EXFUN
inline void __xcore_s_chan_check_ct_end(resource_t c)
{
  asm volatile("chkct res[%0], " _XCORE_STR(XS1_CT_END) :: "r" (c));
}

// Manage direction changes.
// As specified in the Tools Development Guide, the last_out state is managed
// to control when CT_END tokens are sent or expected.
_XCORE_EXFUN
inline void __xcore_t_chan_change_to_input(transacting_chanend_t *tc)
{
  if (tc->last_out) {
    __xcore_s_chan_out_ct_end(tc->c);
    tc->last_out = 0;
  }
}

_XCORE_EXFUN
inline void __xcore_t_chan_change_to_output(transacting_chanend_t *tc)
{
  if (!tc->last_out) {
    __xcore_s_chan_check_ct_end(tc->c);
    tc->last_out = 1;
  }
}
