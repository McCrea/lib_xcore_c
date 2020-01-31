// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

#include <xcore/_support/xcore_c_feature_test_impl.h>

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#define _XCORE_STR1(...) #__VA_ARGS__
#define _XCORE_STR(x) _XCORE_STR1(x)

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_CODE_ALIGNMENT   4
#else
#define _XCORE_CODE_ALIGNMENT   2
#endif

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_STACK_ALIGN(n)   ((n+1)/2)*2
#else
#define _XCORE_STACK_ALIGN(n)   n
#endif

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_ENTSP(n)   nop; entsp n
#else
#define _XCORE_ENTSP(n)   entsp n
#endif

#ifdef _XCORE_HAS_DUAL_ISSUE
#define _XCORE_SINGLE_ISSUE   nop; entsp 0
#else
#define _XCORE_SINGLE_ISSUE
#endif

