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
 *  \li Otherwise, if \c LIBXCORE_XASSERT_IS_ASSERT is \b not defined then the assertions 
 *      will be 'hardware assisted' and will trap on failure without printing any
 *      diagnostic information;
 *  \li Otherwise (i.e. \c LIBXCORE_XASSERT_IS_ASSERT is defined and \c NDEBUG is not) the
 *      assertions will be implemented in terms of the C standard library \c assert.
 */

#include <xcore/_support/xcore_feature_test_impl.h>
#include <xcore/_support/xcore_hardware_assert.h>

#define _XCORE_XASSERT_EMPTY
#define _XCORE_XASSERT_HIDE(_X)


#ifndef _XCORE_HAS_REFERENCE_CLOCK
#define _XCORE_XASSERT_NOT_AFTER(_C) ((void)0)
#else
/** \brief Defined if (and only if) timing assertions are available on the current platform.
 *  \note The definition of this macro does not necessarily mean that timing assertions will
           be effective as they may be disabled with \c NDEBUG.
 */
#define LIBXCORE_HAS_TIMING_ASSERTIONS
#endif

#if defined(NDEBUG) 

#define _XCORE_XASSERT_TRUE(_C) ((void)0)
#define _XCORE_XASSERT_FALSE(_C) ((void)0)

#ifdef _XCORE_HAS_REFERENCE_CLOCK
#define _XCORE_XASSERT_NOT_AFTER(_C) ((void)0)
#endif

#elif !defined(LIBXCORE_XASSERT_IS_ASSERT)

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

#ifdef _XCORE_HAS_REFERENCE_CLOCK
#define _XCORE_XASSERT_NOT_AFTER(CONDITION) \
  do { __xcore_elate((CONDITION)); } while(0)
#endif

#else // xassert is assert
#include <assert.h>

#define _XCORE_XASSERT_TRUE(CONDITION) assert((CONDITION)) _XCORE_XASSERT_HIDE(#CONDITION)
#define _XCORE_XASSERT_FALSE(CONDITION) assert(!(CONDITION)) _XCORE_XASSERT_HIDE(#CONDITION)

#ifdef _XCORE_HAS_REFERENCE_CLOCK
#define _XCORE_XASSERT_NOT_AFTER(CONDITION) assert(__xcore_not_after_reference_time(condition)) _XCORE_XASSERT_HIDE(#CONDITION)
#endif

#endif


/** \brief Assert that a given expression evaluates true
 *
 *  Asserts that \a condition is true (nonzero). The actual behaviour when condition is false
 *  depends on the which configuration macros are defined.
 *
 *  \attention \a condition should not have side effects as these will not be executed when
 *             assertions are ineffective.
 *
 *  \param condition  The expression which is expected to evaluate true
 */
#define xassert(condition) _XCORE_XASSERT_TRUE(condition) _XCORE_XASSERT_HIDE(#condition)

/** \brief Assert that a given expression evaluates false
 * 
 *  Asserts that \a condition is false (0). The actual behaviour when condition is true
 *  depends on the which configuration macros are defined.
 *
 *  \attention \a condition should not have side effects as these will not be executed when
 *             assertions are ineffective.
 *
 *  \param condition  The expression which is expected to evaluate true
 */
#define xassert_not(condition) _XCORE_XASSERT_FALSE(condition) _XCORE_XASSERT_HIDE(#condition)

/** \brief Assert that the given timestamp is not in the past.
 *
 *  On XS2 and onwards this macro implements a timing assertion based on the reference clock.
 *  When \c LIBXCORE_XASSERT_IS_ASSERT is defined an approximation of the 'hardware' assert's condition
 *  is used however this may be less accurate as checking the timestamp cannot be performed as a
 *  single instruction.
 *
 *  The macro LIBXCORE_HAS_TIMING_ASSERTIONS will be defined if and only if this assertion can be
 *  effective on the current platform.
 *
 *  \attention \a timestamp should not have side effects as these will not be executed when
 *             assertions are ineffective (including when timing assertions are not available
 *             on the current platform).
 *  \warning On XS1 devices this assertion will have no effect.
 *  
 *  \param timestamp  The timestamp which is expected not to be in the past
 */
#define xassert_not_after(timestamp) _XCORE_XASSERT_NOT_AFTER(timestamp) _XCORE_XASSERT_HIDE(#timestamp)

