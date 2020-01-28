// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#define _XMM_XASSERT_EMPTY
#define _XMM_XASSERT_HIDE(_X)

#if defined(NDEBUG) 

#define _XMM_XASSERT_TRUE(_C) ((void)0)
#define _XMM_XASSERT_FALSE(_C) ((void)0)
#define _XMM_XASSERT_NOT_AFTER(_C) ((void)0)

#elif !defined(XCORE_C_XASSERT_IS_ASSERT)

#include <xcore/_support/xcore_c_hardware_assert.h>

#define _XMM_XASSERT_TRUE(CONDITION) \
  do { \
    __xcore_c_ecallf((CONDITION)); \
    __builtin_assume((CONDITION)); \
  } while (0)

#define _XMM_XASSERT_FALSE(CONDITION) \
  do { \
    __xcore_c_ecallt((CONDITION)); \
    __builtin_assume(!(CONDITION)); \
  } while(0)

#define _XMM_XASSERT_NOT_AFTER(CONDITION) \
  do { __xcore_c_elate((CONDITION)); } while(0)

#else // xassert is assert
#include <assert.h>
//#include <xcore/the_header_for_reference_time.h> /TODO

#define _XMM_XASSERT_TRUE(CONDITION) assert((CONDITION)) _XMM_XASSERT_HIDE(#CONDITION)
#define _XMM_XASSERT_FALSE(CONDITION) assert(!(CONDITION)) _XMM_XASSERT_HIDE(#CONDITION)
#define _XMM_XASSERT_NOT_AFTER(CONDITION) assert(__xcore_c_current_time() <= (condition)) _XMM_XASSERT_HIDE(#CONDITION)

#endif

#define xassert(condition) _XMM_XASSERT_TRUE(condition) _XMM_XASSERT_HIDE(#condition)
#define xassert_not(condition) _XMM_XASSERT_FALSE(condition) _XMM_XASSERT_HIDE(#condition)
#define xassert_not_after(timestamp) _XMM_XASSERT_NOT_AFTER(timestamp) _XMM_XASSERT_HIDE(#timestamp)

