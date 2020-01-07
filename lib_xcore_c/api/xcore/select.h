// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_select_h__
#define __xcore_c_select_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <xcore/_support/xcore_c_resource_impl.h>

#if defined(XCC_VERSION_MAJOR) || defined(NO_XCLANG_ADDITIONS)
#include <xcore/_support/xcore_c_select_impl_xcc.h>
#else
#include <xcore/_support/xcore_c_select_impl_xclang.h>
#endif

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

/** Disable all select events on this logical core.
 *
 *  This function is called before starting to configure select events for
 *  a new event loop.
 *  This will ensure that no events set up by other code will be triggered
 *
 *  This affect events setup using *res*_setup_select() and
 *  *res*_setup_select_callback() but not *res*_setup_interrupt_callback()
 *
 *  \return     error_none
 */
inline void select_disable_trigger_all(void)
{
  _select_disable_trigger_all();
}

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




#define SELECT_RESET _XMM_SELECT_RESET_I
#define CASE_THEN(RES, LABEL) _XMM_SELECT_RES(RES, LABEL, _XMM_GUARD_NONE)
#define CASE_GUARD_THEN(RES, GUARD_EXPR, LABEL) _XMM_SELECT_RES(RES, LABEL, _XMM_GUARD_TRUE(GUARD_EXPR))
#define CASE_NGUARD_THEN(RES, GUARD_EXPR, LABEL) _XMM_SELECT_RES(RES, LABEL, _XMM_GUARD_FALSE(GUARD_EXPR))
#define DEFAULT_THEN(LABEL) _XMM_SELECT_DEFAULT(LABEL, _XMM_GUARD_NONE)
#define DEFAULT_GUARD_THEN(GUARD_EXPR, LABEL) _XMM_SELECT_DEFAULT(LABEL, _XMM_GUARD_TRUE(GUARD_EXPR))
#define DEFAULT_NGUARD_THEN(GUARD_EXPR, LABEL) _XMM_SELECT_DEFAULT(LABEL, _XMM_GUARD_FALSE(GUARD_EXPR))

#endif // !defined(__XC__)

#endif // __xcore_c_select_h__
