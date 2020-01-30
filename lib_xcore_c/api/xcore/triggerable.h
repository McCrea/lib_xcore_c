// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief Functions applicable to Chanends, Ports and Timers for configuring events and
 *         interrupts.
 *  \see xcore/interrupt.h
 */

#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_triggerable_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>


/** Setup interrupt event on a resource.
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
void triggerable_setup_interrupt_callback(resource_t res, void *data, interrupt_callback_t func)
{
  __xcore_resource_setup_interrupt_callback(res, data, func);
}

#define TRIGGERABLE_SETUP_EVENT_VECTOR(resource, label) \
  _XCORE_TRIGGERABLE_SETUP_EVENT_VECTOR(resource, label)

#define TRIGGERABLE_WAIT_EVENT(labels...) \
  _XCORE_TRIGGERABLE_WAIT_EVENT(labels)

#define TRIGGERABLE_TAKE_EVENT(labels...) \
  _XCORE_TRIGGERABLE_TAKE_EVENT(labels)

void triggerable_enable_trigger(resource_t res)
{
  __xcore_resource_event_enable_unconditional(res);
}

void triggerable_disable_trigger(resource_t res)
{
  __xcore_resource_event_disable_unconditional(res);
}

void triggerable_set_trigger_enabled(resource_t res, _Bool enabled)
{
  __xcore_resource_event_enable_if_true(res, enabled);
}


