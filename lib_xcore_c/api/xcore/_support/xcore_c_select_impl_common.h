// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_macros.h>
#include <xcore/_support/xcore_c_meta_macro.h>
#include <xcore/_support/xcore_c_resource_impl.h>

// These old functions support the XCC implementation of the new select interface.

/** Starting value to use for the enum_id
 *
 *  The enum_id is passed to the *res*_setup_select() and returned by
 *  select_wait() et al
 *
 *  On XS1 the environment vectors (EVs) are only 16-bit and bit 16 will be set
 *  to 1 as it is expected to be used as a memory address.
*/
#if defined(__XS2A__)
#define ENUM_ID_BASE 0
#else
#define ENUM_ID_BASE 0x10000
#endif

/** Wait for a select event to trigger.
 *
 *  This function waits for an event to trigger and then returns the value the
 *  user has registered with the resource that triggered the event.
 *
 *  \returns  The enum_id registered with the resource when events were enabled
 */
uint32_t select_wait(void);

/** Check whether any select events have triggered, otherwise return.
 *
 *  This function tests for an event to being ready. If there is one ready then
 *  it returns the enum_id the user has registered with the resource that
 *  triggered the event. If no events are ready then returns the no_wait_id
 *  passed in by the user.
 *
 *  **select_callback_t events are handled, but are not considered 'select events'**
 *
 *  \param no_wait_id  The enum_id to return if no 'select event' is triggered
 *
 *  \returns  The enum_id registered with the resource which triggered an event
 *            or the no_wait_id passed in if no event fired
 */
uint32_t select_no_wait(uint32_t no_wait_id);

/** Wait for an select event from a list of resources using an ordered enable sequence
 *
 *  This function:
 *    - Starts by clearing all select events that have been configured for this core.
 *      This includes select_callback_t functions but not interrupt_callback_t functions.
 *    - Enables select events on each resource in turn so that there is a defined
 *      order in which pending events will be taken
 *
 *  **Enabled select_callback_t resources will be taken, but will not terminate
 *  the process. A user may wish to place these at the front of the list**
 *
 *  \param ids  Null-terminated list of resources to enable events on
 *
 *  \returns    The enum_id registered with the resource which triggers an event
 *
 *  \exception  ET_LOAD_STORE         invalid *ids[]* argument.
 */
uint32_t select_wait_ordered(const resource_t ids[]);

/** Wait for a select event from a list of resources using an ordered enable sequence
 *
 *  This function does the same as select_wait_ordered, but will return the
 *  no_wait_id if no select event fires by the end of the enabling sequence.
 *
 *  **select_callback_t events are handled, but are not considered 'select events'**
 *
 *  \param no_wait_id  The enum_id to return if no 'select event' is triggered
 *  \param ids         Null-terminated list of resources to enable events on
 *
 *  \returns  The enum_id registered with the resource which triggered an event
 *            or the no_wait_id passed in if no event fired
 *
 *  \exception  ET_LOAD_STORE         invalid *ids[]* argument.
 */
uint32_t select_no_wait_ordered(uint32_t no_wait_id, const resource_t ids[]);



// Start of support for new style

_XCORE_EXFUN
inline void __xcore_select_disable_trigger_all(void)
{
  asm volatile("clre");
}

_XCORE_EXFUN
inline void __xcore_select_event_enable_unconditional(const resource_t resource)
{
  asm volatile(
    "eeu res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource));
}

_XCORE_EXFUN
inline void __xcore_select_event_disable_unconditional(const resource_t resource)
{
  asm volatile(
    "edu res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource));
}

_XCORE_EXFUN
inline void __xcore_select_event_set_enable(const resource_t resource, const int condition)
{
  asm volatile(
    "eet %[cond], res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource) , [cond] "r"(condition));
}

_XCORE_EXFUN
inline void __xcore_select_event_set_enable_inv(const resource_t resource, const int condition)
{
  asm volatile(
    "eef %[cond], res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource) , [cond] "r"(condition));
}

_XCORE_EXFUN
inline void __xcore_select_event_enable_if_true(const resource_t resource, const int condition)
{
  if (__builtin_constant_p(condition))
  {
    if (condition)
    {
      __xcore_select_event_enable_unconditional(resource);
    }
    else
    {
      __xcore_select_event_disable_unconditional(resource);
    }
  }
  else 
  {
    __xcore_select_event_set_enable(resource, condition);
  }
}

_XCORE_EXFUN
inline void __xcore_select_event_enable_if_false(const resource_t resource, const int condition)
{
  if (__builtin_constant_p(condition))
  {
    if (condition)
    {
      __xcore_select_event_disable_unconditional(resource);
    }
    else
    {
      __xcore_select_event_enable_unconditional(resource);
    }
  }
  else 
  {
    __xcore_select_event_set_enable_inv(resource, condition);
  }
}

_XCORE_EXFUN
inline void __xcore_register_event_vector(const resource_t resource, void * const vector)
{
  register void * const vector_reg asm("r11") = vector; 
  asm volatile (
    "setv res[%[res]], %[target] \n"
    :
    : [res] "r"(resource), [target] "r"(vector_reg));
}

#define _XCORE_GUARD_NONE _XCORE_GTYPE_NONE, /* Guard expression should never be used */
#define _XCORE_GUARD_TRUE(EXPR) _XCORE_GTYPE_TRUE, EXPR
#define _XCORE_GUARD_FALSE(EXPR) _XCORE_GTYPE_FALSE, EXPR

