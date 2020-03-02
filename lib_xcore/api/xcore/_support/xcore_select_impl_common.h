// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_macros.h>
#include <xcore/_support/xcore_meta_macro.h>
#include <xcore/_support/xcore_resource_impl.h>

#ifdef __cplusplus
extern "C" {
#endif


_XCORE_EXFUN
inline void __xcore_select_disable_trigger_all(void) _XCORE_NOTHROW
{
  asm volatile("clre");
}


#define _XCORE_GUARD_NONE _XCORE_GTYPE_NONE, /* Guard expression should never be used */
#define _XCORE_GUARD_TRUE(EXPR) _XCORE_GTYPE_TRUE, EXPR
#define _XCORE_GUARD_FALSE(EXPR) _XCORE_GTYPE_FALSE, EXPR

#ifdef __cplusplus
}
#endif

