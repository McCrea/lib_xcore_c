// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_feature_test_impl.h>

#ifdef __cplusplus
extern "C" {
#endif


// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_CODE_ALIGNMENT   4
#else
#define _XCORE_CODE_ALIGNMENT   2
#endif

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_STACK_ALIGN(__n)   ((__n+1)/2)*2
#else
#define _XCORE_STACK_ALIGN(__n)   __n
#endif

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_ENTSP(__n)   nop; entsp __n
#else
#define _XCORE_ENTSP(__n)   entsp __n
#endif

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_STACK_ALIGN_REQUIREMENT 8
#define _XCORE_STACK_ALIGN_REQUIREMENT_WORDS 2
#else
#define _XCORE_STACK_ALIGN_REQUIREMENT 4
#define _XCORE_STACK_ALIGN_REQUIREMENT_WORDS 1
#endif

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_SINGLE_ISSUE   nop; entsp 0
#else
#define _XCORE_SINGLE_ISSUE
#endif

#if defined(__XS2A__)
#define _XCORE_ENUM_ID_BASE 0
#else
#define _XCORE_ENUM_ID_BASE 0x10000
#endif

#ifdef __cplusplus
}
#endif

