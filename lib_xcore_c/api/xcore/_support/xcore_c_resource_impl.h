// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_macros.h>
#include "xassert.h" //TODO: remove


/** wrapped interrupt callback function
 *
 *  This is an opaque type returned by the INTERRUPT_CALLBACK() macro.
 *
 *  Users must not access its raw underlying type.
 */
typedef void(*interrupt_callback_t)(void);

extern void _select_non_callback(void);  // Implemented in xcore_c_select.S

_XCORE_C_EXFUN
inline void _resource_enable_trigger(resource_t r)
{
  asm volatile("eeu res[%0]" :: "r" (r));
}

_XCORE_C_EXFUN
inline void _resource_disable_trigger(resource_t r)
{
  asm volatile("edu res[%0]" :: "r" (r));
}

_XCORE_C_EXFUN
inline void _resource_setup_interrupt_callback(resource_t r, void *data, interrupt_callback_t intrpt)
{
  _resource_setup_callback(r, data, intrpt, 0xA);  // Raise interrupts instead of events
}

_XCORE_C_EXFUN
inline void _resource_setup_select(resource_t r, uint32_t value)
{
  _resource_setup_select_callback(r, (void*)value, _select_non_callback);
}

#define _RESOURCE_ALLOC(res, id) asm volatile( "getr %0, %1" : "=r" (res) : "n" (id))

_XCORE_C_EXFUN
inline void _resource_free(resource_t r)
{
  asm volatile("freer res[%0]" :: "r" (r));
}

#define _RESOURCE_SETCI(res, c) asm volatile( "setc res[%0], %1" :: "r" (res), "n" (c))
