// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#define _XCORE_XASSERT_EMPTY
#define _XCORE_XASSERT_HIDE(_X)

#if defined(NDEBUG) 

#define _XCORE_XASSERT_TRUE(_C) ((void)0)
#define _XCORE_XASSERT_FALSE(_C) ((void)0)
#define _XCORE_XASSERT_NOT_AFTER(_C) ((void)0)

#elif !defined(XCORE_C_XASSERT_IS_ASSERT)

#include <xcore/_support/xcore_c_hardware_assert.h>

#define _XCORE_XASSERT_TRUE(CONDITION) \
  do { \
    __xcore_ecallf((CONDITION)); \
    __builtin_assume((CONDITION)); \
  } while (0)

#define _XCORE_XASSERT_FALSE(CONDITION) \
  do { \
    __xcore_ecallt((CONDITION)); \
    __builtin_assume(!(CONDITION)); \
  } while(0)

#define _XCORE_XASSERT_NOT_AFTER(CONDITION) \
  do { __xcore_elate((CONDITION)); } while(0)

#else // xassert is assert
#include <assert.h>
//#include <xcore/the_header_for_reference_time.h> /TODO

#define _XCORE_XASSERT_TRUE(CONDITION) assert((CONDITION)) _XCORE_XASSERT_HIDE(#CONDITION)
#define _XCORE_XASSERT_FALSE(CONDITION) assert(!(CONDITION)) _XCORE_XASSERT_HIDE(#CONDITION)
#define _XCORE_XASSERT_NOT_AFTER(CONDITION) assert(__xcore_current_time() <= (condition)) _XCORE_XASSERT_HIDE(#CONDITION)

#endif

#define xassert(condition) _XCORE_XASSERT_TRUE(condition) _XCORE_XASSERT_HIDE(#condition)
#define xassert_not(condition) _XCORE_XASSERT_FALSE(condition) _XCORE_XASSERT_HIDE(#condition)
#define xassert_not_after(timestamp) _XCORE_XASSERT_NOT_AFTER(timestamp) _XCORE_XASSERT_HIDE(#timestamp)

