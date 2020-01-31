// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief API for using hardware timers to measure and wait time
 */

#include <stdint.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_hwtimer_impl.h>
#include <xcore/_support/xcore_resource_impl.h>
#include <xcore/_support/xcore_reference_time.h>

/** brief Hardware timer handle type. */
typedef resource_t hwtimer_t;


/** \brief Deallcoates the hardware timer automatically allocated
 *
 *  for xC use. Each logical core is allocated a hardware timer that is
 *  multiplexed and used by the xC 'timer' interface. This multiplexed
 *  timer is not accessible from C.
 *  If the logical core is not running any xC code, or any xC code
 *  is not making use of the 'timer' resource type, the allocated hardware
 *  timer may be retrieved for use as a hwtimer_t.
 *
 *  \note This call must be paired with a call to hwtimer_realloc_xc_timer()
 *  prior to the logical core completing its tasks.
 *
 *  \note The xScope link also requires a hardware timer.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   timer has already been deallocated.
 */
_XCORE_EXFUN
inline void hwtimer_free_xc_timer(void)
{
  __xcore_hwtimer_free_xc_timer();
}

/** \brief Reallcoates a logical core's xC hardware timer that was
 *
 *  deallocated by a call to hwtimer_free_xc_timer().
 *
 *  \attention There must be an available hw timer when this call is made,
 *  otherwise a trap will be raised.
 *
 *  \exception  ET_ECALL   no available hw timer, reallocation failed.
 */
_XCORE_EXFUN
inline void hwtimer_realloc_xc_timer(void)
{
  __xcore_hwtimer_realloc_xc_timer();
}

/** \brief Allocates a hardware timer.
 * 
 *  If there are no timers available then 0 is returned.
 *  \note When the timer is no longer required, hwtimer_free() should be called
 *  to deallocate it.
 *
 *  \return     Timer handle to the allocated timer
 */
_XCORE_EXFUN
inline hwtimer_t hwtimer_alloc()
{
  return __xcore_hwtimer_alloc();
}

/** \brief Deallocate a timer.
 *
 *  \param t  The timer to be freed
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
_XCORE_EXFUN
inline void hwtimer_free(hwtimer_t t)
{
  __xcore_hwtimer_free(t);
}

/** \brief Get the current time from the timer.
 *
 *  If there is a trigger time setup, the call will stall until after the trigger time.
 *  For select and interrupt event, calling hwtimer_get_time() will clear the event.
 *
 *  \return The time value (a 32-bit value)
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
_XCORE_EXFUN
inline uint32_t hwtimer_get_time(hwtimer_t t)
{
  return __xcore_hwtimer_get_time(t);
}

/** \brief Get the trigger time value.
 *
 *  The trigger time value is set using hwtimer_set_trigger_time().
 *  The trigger may be cleared using hwtimer_clear_trigger_time().
 *
 *  \param t  The timer whose time value is requested.
 *  \return   The time value
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
_XCORE_EXFUN
inline uint32_t hwtimer_get_trigger_time(hwtimer_t t)
{
  return __xcore_hwtimer_get_trigger_time(t);
}

/** \brief Setup an event trigger on a timer.
 *
 *  \note This will cause hwtimer_get_time() to pause until the specified time.
 *  The trigger may be cleared using hwtimer_clear_trigger_time().
 *
 *  \note hwtimer_wait_until(), hwtimer_delay(), hwtimer_setup_select()
 *  hwtimer_setup_select_callback() and hwtimer_setup_interrupt_callback()
 *  call hwtimer_set_trigger_time()
 *
 *  \param t     The timer to setup a event trigger on.
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
_XCORE_EXFUN
inline void hwtimer_set_trigger_time(hwtimer_t t, uint32_t time)
{
  __xcore_hwtimer_set_trigger_time(t, time);
}

/** \brief Change the time at which a timer trigger will fire.
 *
 *  This function modifies the time at which a previously setup trigger fires.
 *  It is used to set a new trigger time after a select or interrupt event has occurred.
 *
 *  \param t     The timer to change
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
_XCORE_EXFUN
inline void hwtimer_change_trigger_time(hwtimer_t t, uint32_t time)
{
  __xcore_hwtimer_change_trigger_time(t, time);
}

/** \brief Clear an event trigger on a timer.
 *
 *  Makes sure no triggers are setup on a timer.
 *  Should be called when a timer is no longer being used for select and interrupt events.
 *  \note Both hwtimer_wait_until() and hwtimer_delay() call hwtimer_clear_trigger_time().
 *
 *  \param t  The timer to tear down events on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */

_XCORE_EXFUN
inline void hwtimer_clear_trigger_time(hwtimer_t t)
{
  __xcore_hwtimer_clear_trigger_time(t);
}

/** \brief Wait until after a specified time.
 *
 *  \attention This will destroy any select or interrupt event triggers set on this resource.
 *
 *  \param t      The timer to use for timing
 *  \param until  The time to wait until
 *
 *  \return The time we actually waited until
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
_XCORE_EXFUN
inline uint32_t hwtimer_wait_until(resource_t t, uint32_t until)
{
  __xcore_hwtimer_set_trigger_time(t, until);
  uint32_t now = __xcore_hwtimer_get_time(t);
  __xcore_hwtimer_clear_trigger_time(t);
  return now;
}

/** \brief Delay for a specified time using a specific timer.
 *
 *  \attention This will destroy any select or interrupt event triggers set on this resource
 *
 *  \param t       The timer resource to use
 *  \param period  The amount of time to wait (in reference time ticks, usually 10ns steps)
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *\*now* argument.
 */
_XCORE_EXFUN
inline void hwtimer_delay(resource_t t, uint32_t period)
{
  uint32_t start = __xcore_hwtimer_get_time(t);
  uint32_t until = start + period;
  __xcore_hwtimer_set_trigger_time(t, until);
  (void)__xcore_hwtimer_get_time(t);
  __xcore_hwtimer_clear_trigger_time(t);
}

#ifdef _XCORE_HAS_REFERENCE_CLOCK
#define LIBXCORE_HWTIMER_HAS_REFERENCE_TIME
#endif

/** \brief get the chip reference time
 *
 *  Gets the current reference time without requiring an allocated timer on chips where a
 *  reference time is available.
 *  This can be tested with \a LIBXCORE_HWTIMER_HAS_REFERENCE_TIME which will be defined
 *  if and only if a refence time is available.
 *  If no reference time is available then 0 is returned.
 *
 *  \return The reference time
 */
_XCORE_EXFUN
inline uint32_t get_reference_time(void)
{
#ifdef LIBXCORE_HWTIMER_HAS_REFERENCE_TIME
  return __xcore_get_reference_time();
#else
  return 0;
#endif
}

