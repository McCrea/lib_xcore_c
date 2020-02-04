// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief Functions applicable to Chanends, Ports and Timers for configuring events and
 *         interrupts.
 *
 *  Low level API for handling events and interrupts. It is generally recommended to use the
 *  SELECT API instead.
 * 
 *  \note A resource may only be configured for interrupts \b or events at a given time.
 *        Configuring interrupts for a resource may overwrite any event configuration and
 *        configuring events may overwrite any interrupt configurataion. A trigger controls
 *        the condition for both interrups and for events (whichever is enabled for the resource).
 *
 *  \see \li xcore/select.h \li xcore/interrupt.h
 */

#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_triggerable_impl.h>
#include <xcore/_support/xcore_resource_impl.h>


/** \brief Setup interrupt event on a resource.
 *
 *  Once the interrupts is setup you need to call port_enable_trigger() to enable it.
 *
 *  \param res       The resource to setup the interrupt event on.
 *
 *  \param[in] data  The value to be passed to the interrupt_callback_t function
 *                    On XS1 bit 16 must be set (see ENUM_ID_BASE)
 *  \param func       The interrupt_callback_t function to handle events
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chanend, port or timer
 *  \exception  ET_RESOURCE_DEP       another core is actively using the resource.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in data.
 */
_XCORE_EXFUN
inline void triggerable_setup_interrupt_callback(resource_t res, void *data, interrupt_callback_t func)
{
  __xcore_resource_setup_interrupt_callback(res, data, func);
}

/** \brief Configure the vector which handles events on a given resource
 *
 *  Expands to a statement which configures the event vector for the given \a resource
 *  such that handling an event on that resource causes control to transfer to the
 *  given \a label.
 *
 *  \note This will overwrite any interrupt vector which has been set on the resource.
 * 
 *  \param resource  The resource to configure
 *  \param label     The label to jump to when an event is handled on \a resource
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chanend, port or timer
 *  \exception  ET_RESOURCE_DEP       another core is actively using the resource.
 */
#define TRIGGERABLE_SETUP_EVENT_VECTOR(resource, label) \
  _XCORE_TRIGGERABLE_SETUP_EVENT_VECTOR(resource, label)

/** \brief Wait for a configured and enabled event to occur
 *
 *  Expands to a statements which waits for a configured event to occur and jumps to its
 *  event vector. Note that any possible event vector which may be jumped to (i.e. the labels
 *  configured as event vectors on all resources where events are enabled and an event may occur)
 *  must be passed as arguments otherwise the effect is undefined. Passing labels which cannot
 *  be reached by an enabled event may be ill formed if jumping to that label would be ill formed
 *  and may result in sub-optimal performance.
 *
 *  For a resource to raise an event:
 *  \li Its trigger must be enabled with triggerable_enable_trigger() or triggerable_set_trigger_enabled()
 *  \li Its trigger must occur; for a chanend this happens when data is available to be read, for
 *      ports and timers this will depend on the trigger which has been configured using the respective API.
 *  If at the point an event occurs the event vector has not been configured using 
 *  TRIGGERABLE_SETUP_EVENT_VECTOR() then handling the event will have undefined behaviour.
 *
 *  \attention  The expansion of this macro will block until the trigger occurs on a resource
 *              where events are enabled. If no events are enabled then this can never complete.
 *
 *  \param labels  The labels configured as event vectors for all events which may occur.
 */
#define TRIGGERABLE_WAIT_EVENT(labels...) \
  _XCORE_TRIGGERABLE_WAIT_EVENT(labels)

/** \brief Jump to an event vector if an event is ready
 *
 *  Has the same effect as TRIGGERABLE_WAIT_EVENT() except that if no event is waiting then
 *  this expansion does not block.
 *
 *  \param labels Labels which may be jumped to if an event is ready
 */
#define TRIGGERABLE_TAKE_EVENT(labels...) \
  _XCORE_TRIGGERABLE_TAKE_EVENT(labels)

/** \brief  Enable the trigger for a given resource
 *
 *  This will allow the resource the generate events or interrupts when its trigger occurs.
 *
 *  \param res  Resource to enable the trigger of
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chanend, port or timer
 *  \exception  ET_RESOURCE_DEP       another core is actively using the resource.
 */
_XCORE_EXFUN
inline void triggerable_enable_trigger(resource_t res)
{
  __xcore_resource_event_enable_unconditional(res);
}

/** \brief  Disable the trigger for a given resource
 *
 *  This prevents the resource generating events or interrupts.
 *
 *  \param res  Resource to disable the trigger of
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chanend, port or timer
 *  \exception  ET_RESOURCE_DEP       another core is actively using the resource.
 */
_XCORE_EXFUN
inline void triggerable_disable_trigger(resource_t res)
{
  __xcore_resource_event_disable_unconditional(res);
}

/** \brief Set the trigger enabled or disabled on a given resource
 *
 *   Has the same effect as calling triggerable_enable_trigger() or triggerable_disable_trigger()
 *   depending on the value of \a enabled. However, this may perform better when the value of
 *   enabled is not statically known.
 *
 *  \param res     The resource to enaable or disable the trigger of
 *  \param enabled State to set on the trigger - if true it is enabled, otherwise it is disabled
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chanend, port or timer
 *  \exception  ET_RESOURCE_DEP       another core is actively using the resource.
 */
_XCORE_EXFUN
inline void triggerable_set_trigger_enabled(resource_t res, _Bool enabled)
{
  __xcore_resource_event_enable_if_true(res, enabled);
}

/** \brief Disables all triggers in the current thread and masks interrupts
 * 
 *  Disables the triggers on all resources in use by the current thread and disabled
 *  interrupts as if with interrupt_mask_all().
 */
_XCORE_EXFUN
inline void triggerable_disable_all(void)
{
  __triggerable_disable_all();
} 