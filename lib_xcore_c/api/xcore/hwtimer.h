// Copyright (c) 2016, XMOS Ltd, All rights reserved

/** \file
 *  \brief API for using hardware timers to measure and wait time
 */

#ifndef __xcore_c_hwtimer_h__
#define __xcore_c_hwtimer_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <xcore/_support/xcore_c_hwtimer_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>

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
inline void hwtimer_free_xc_timer(void)
{
  _hwtimer_free_xc_timer();
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
inline void hwtimer_realloc_xc_timer(void)
{
  _hwtimer_realloc_xc_timer();
}

/** \brief Allocates a hardware timer.
 * 
 *  If there are no timers available then 0 is returned.
 *  \note When the timer is no longer required, hwtimer_free() should be called
 *  to deallocate it.
 *
 *  \return     Timer handle to the allocated timer
 */
inline hwtimer_t hwtimer_alloc()
{
  return _hwtimer_alloc();
}

/** \brief Deallocate a timer.
 *
 *  \param t  The timer to be freed
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline void hwtimer_free(hwtimer_t t)
{
  _hwtimer_free(t);
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
inline uint32_t hwtimer_get_time(hwtimer_t t)
{
  return _hwtimer_get_time(t);
}

/** \brief Setup an event trigger on a timer.
 *
 *  \note This will cause hwtimer_get_time() to pause until the specified time.
 *  The trigger may be cleared using hwtimer_clear_trigger_time().
 *
 *  \note hwtimer_wait_until(), hwtimer_delay(), hwtimer_setup_select()
 *  hwtimer_setup_select_callback() and hwtimer_setup_interrupt_callback()
 *  call hwtimer_set_trigger_time()**
 *
 *  \param t     The timer to setup a event trigger on.
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline void hwtimer_set_trigger_time(hwtimer_t t, uint32_t time)
{
  _hwtimer_set_trigger_time(t, time);
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
inline void hwtimer_change_trigger_time(hwtimer_t t, uint32_t time)
{
  _hwtimer_change_trigger_time(t, time);
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

inline void hwtimer_clear_trigger_time(hwtimer_t t)
{
  _hwtimer_clear_trigger_time(t);
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
inline uint32_t hwtimer_wait_until(hwtimer_t t, uint32_t until)
{
  _hwtimer_set_trigger_time(t, until);
  uint32_t now = _hwtimer_get_time(t);
  _hwtimer_clear_trigger_time(t);
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
inline void hwtimer_delay(hwtimer_t t, uint32_t period)
{
  uint32_t start = _hwtimer_get_time(t);
  uint32_t until = start + period;
  _hwtimer_set_trigger_time(t, until);
  (void)_hwtimer_get_time(t);
  _hwtimer_clear_trigger_time(t);
}

/** \brief Setup interrupt event on a timer.
 *
 *  \note Once the event is setup you need to call hwtimer_enable_trigger() to enable it.
 *
 *  \param t     The timer to setup the events on
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *  \param data  The value to be passed to the interrupt_callback_t function
 *  \param func  The interrupt_callback_t function to handle the events
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline void hwtimer_setup_interrupt_callback(hwtimer_t t, uint32_t time, void *data, interrupt_callback_t func)
{
  _resource_setup_interrupt_callback(t, data, func);
  _hwtimer_set_trigger_time(t, time);
}

/** \brief Enable select & interrupt events on a timer.
 *
 *  \note Prior to enabling, hwtimer_setup_select(), hwtimer_setup_select_callback() or
 *         hwtimer_setup_interrupt_callback() must have been called.
 *
 * \note Events can be temporarily disabled and re-enabled using hwtimer_disable_trigger()
 *       and hwtimer_enable_trigger().
 *       When the event fires, hwtimer_get_time() must be called to clear the event.
 *       The time of the next event is set using hwtimer_change_trigger_time().
 *  
 *  \param t  The timer to enable events on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline void hwtimer_enable_trigger(hwtimer_t t)
{
  _resource_enable_trigger(t);
}

/** \brief Disable select & interrupt events for a given timer.
 *
 *  Prevents any further select or interrupt events being triggered
 *  by the given timer.
 *
 *  \attention This does not clear the trigger setup
 *
 *  \param t  The timer to disable events on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline void hwtimer_disable_trigger(hwtimer_t t)
{
  _resource_disable_trigger(t);
}

#endif // !defined(__XC__)

#endif // __xcore_c_hwtimer_h__
