// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include <stddef.h>
#include <xs1.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_resource_impl.h>

_XCORE_EXFUN
inline void __xcore_port_set_transfer_width(resource_t p, size_t width)
{
  asm volatile("settw res[%0], %1" :: "r" (p), "r" (width));
}

_XCORE_EXFUN
inline void __xcore_port_enable(unsigned id)
{
  _RESOURCE_SETCI((resource_t)id, XS1_SETC_INUSE_ON);
}

_XCORE_EXFUN
inline void __xcore_port_reset(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INUSE_ON);
}

_XCORE_EXFUN
inline void __xcore_port_free(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INUSE_OFF);
}

_XCORE_EXFUN
inline void __xcore_port_set_buffered(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_BUF_BUFFERS);
}

_XCORE_EXFUN
inline void __xcore_port_set_unbuffered(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_BUF_NOBUFFERS);
}

_XCORE_EXFUN
inline void __xcore_port_set_clock(resource_t p, resource_t clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (p), "r" (clk));
}

_XCORE_EXFUN
inline void __xcore_port_set_inout_data(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_PORT_DATAPORT);
}

_XCORE_EXFUN
inline void __xcore_port_set_out_clock(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_PORT_CLOCKPORT);
}

_XCORE_EXFUN
inline void __xcore_port_set_out_ready(resource_t p, resource_t ready_source)
{
  _RESOURCE_SETCI(p, XS1_SETC_PORT_READYPORT);
  asm volatile("setrdy res[%0], %1" :: "r" (p), "r" (ready_source));
}

_XCORE_EXFUN
inline void __xcore_port_set_invert(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INV_INVERT);
}

_XCORE_EXFUN
inline void __xcore_port_set_no_invert(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_INV_NOINVERT);
}

_XCORE_EXFUN
inline void __xcore_port_set_sample_falling_edge(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_SDELAY_SDELAY);
}

_XCORE_EXFUN
inline void __xcore_port_set_sample_rising_edge(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_SDELAY_NOSDELAY);
}

_XCORE_EXFUN
inline void __xcore_port_set_master(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_MS_MASTER);
}

_XCORE_EXFUN
inline void __xcore_port_set_slave(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_MS_SLAVE);
}

_XCORE_EXFUN
inline void __xcore_port_set_no_ready(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RDY_NOREADY);
}

_XCORE_EXFUN
inline void __xcore_port_set_ready_strobed(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RDY_STROBED);
}

_XCORE_EXFUN
inline void __xcore_port_set_ready_handshake(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RDY_HANDSHAKE);
}

_XCORE_EXFUN
inline int16_t __xcore_port_get_trigger_time(resource_t p)
{
  int16_t ts;
  asm volatile("getts %0, res[%1]" : "=r" (ts) : "r" (p));
  return ts;
}

_XCORE_EXFUN
inline void __xcore_port_set_trigger_time(resource_t p, int16_t t)
{
  asm volatile("setpt res[%0], %1" :: "r" (p), "r" (t));
}

_XCORE_EXFUN
inline void __xcore_port_clear_trigger_time(resource_t p)
{
  asm volatile("clrpt res[%0]" :: "r" (p));
}

_XCORE_EXFUN
inline void __xcore_port_set_trigger_value(resource_t p, uint32_t d)
{
  asm volatile("setd res[%0], %1" :: "r" (p), "r" (d));
}

_XCORE_EXFUN
inline void __xcore_port_set_trigger_in_equal(resource_t p, uint32_t d)
{
  _RESOURCE_SETCI(p, XS1_SETC_COND_EQ);
  __xcore_port_set_trigger_value(p, d);
}

_XCORE_EXFUN
inline void __xcore_port_set_trigger_in_not_equal(resource_t p, uint32_t d)
{
  _RESOURCE_SETCI(p, XS1_SETC_COND_NEQ);
  __xcore_port_set_trigger_value(p, d);
}

_XCORE_EXFUN
inline void __xcore_port_clear_trigger_in(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_COND_NONE);
}

_XCORE_EXFUN
inline uint32_t __xcore_port_peek(resource_t p)
{
  uint32_t data;
  asm("peek %0, res[%1]" : "=r" (data): "r" (p));
  return data;
}

_XCORE_EXFUN
inline void __xcore_port_out(resource_t p, uint32_t data)
{
  asm volatile("out res[%0], %1" :: "r" (p), "r" (data));
}

_XCORE_EXFUN
inline uint32_t __xcore_port_in(resource_t p)
{
  uint32_t data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (p));
  return data;
}

_XCORE_EXFUN
inline uint32_t __xcore_port_out_shift_right(resource_t p, uint32_t data)
{
  // We read-write data
  asm volatile("outshr res[%1], %0" : "+r" (data) : "r" (p));
  return data;
}

_XCORE_EXFUN
inline uint32_t __xcore_port_in_shift_right(resource_t p, uint32_t data)
{
  // We read-write data
  asm volatile("inshr %0, res[%1]" : "+r" (data) : "r" (p));
  return data;
}

_XCORE_EXFUN
inline size_t __xcore_port_endin(resource_t p)
{
  size_t num;
  asm volatile("endin %0, res[%1]" : "=r" (num) : "r" (p));
  return num;
}

_XCORE_EXFUN
inline void __xcore_port_clear_buffer(resource_t p)
{
  _RESOURCE_SETCI(p, XS1_SETC_RUN_CLRBUF);
}
