// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief Provides assertions similar to those defined by the standard C assert.h.
 * 
 *  Assertions provided by this header are intended by behave similarly to the C standard assert.
 *  However by default, when enabled, these macros will expand to 'hardware assisted' assertions
 *  which cause processor exceptions upon failure. 'Hardware assisted' assertions can execute
 *  quickly and do not print any diagnostic information. This makes these assertions suitable
 *  for applications where small code size is desirable.
 *
 *  The behaviour of the (expansion of) macros defined in this header varies depending on
 *  the macros defined at the point it is first included. Specifically:
 *  \li If \c NDEBUG is defined all assertions will expand to <tt>((void)0)</tt>;
 *  \li Otherwise, if \c XCORE_C_XASSERT_IS_ASSERT is \b not defined then the assertions 
 *      will be 'hardware assisted' and will trap on failure without printing any
 *      diagnostic information;
 *  \li Otherwise (i.e. \c XCORE\_XASSERT_IS_ASSERT is defined and \c NDEBUG is not) the
 *      assertions will be implemented in terms of the C standard library \c assert.
 */

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


/** \brief
 *
 *  \param condition
 *

 */
#define xassert(condition) _XCORE_XASSERT_TRUE(condition) _XCORE_XASSERT_HIDE(#condition)

/** \brief
 *
 *  \param condition
 */
#define xassert_not(condition) _XCORE_XASSERT_FALSE(condition) _XCORE_XASSERT_HIDE(#condition)

/** \brief
 *
 *  \attention XS1 does not support this assertion directly, so for XS1 targets this macro
 *             will expand to code which explicitly retrieves the current 
 *  \param timestamp
 */
#define xassert_not_after(timestamp) _XCORE_XASSERT_NOT_AFTER(timestamp) _XCORE_XASSERT_HIDE(#timestamp)

