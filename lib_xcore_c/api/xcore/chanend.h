// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_chanend_h__
#define __xcore_c_chanend_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <xcore/_support/xcore_c_chan_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>

/** Allocate a single streaming_chanend_t.
 *
 *  This function allocates a hardware channel end.
 *  If there are no channel ends available the function returns 0.
 *  When the channel end is no longer required, s_chanend_free() must be called
 *  to deallocate it.
 *
 *  \param c    streaming_chanend_t or zero if none are available.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *\*c* argument.
 */
inline streaming_chanend_t s_chanend_alloc()
{
  return _s_chanend_alloc();
}

/** Deallocate a single streaming_chanend_t.
 *
 *  This function frees the hardware streaming_chanend_t.
 *  The last transfer on the streaming_chanend_t must have been a CT_END token.
 *
 *  \param c    streaming_chanend_t to free.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated streaming_chanend_t,
 *                                    an input/output is pending,
 *                                    or it has not received/sent a CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the streaming_chanend_t.
 *  \exception  ET_LOAD_STORE         invalid *\*c* argument.
 */
inline void s_chanend_free(streaming_chanend_t c)
{
  _s_chanend_free(c);
}

/** Set the destination of a streaming_chanend_t
 *
 *  \param c    streaming_chanend_t to set.
 *
 *  \param dst  Destination streaming_chanend_t.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated streaming_chanend_t.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the streaming_chanend_t.
*/
inline void s_chanend_set_dest(streaming_chanend_t c, streaming_chanend_t dst)
{
  _s_chanend_set_dest(c, dst);
}

/** Convert a chanend to a streaming_chanend_t.
 *
 *  A chanend is always in a safe state for converting into a streaming_chanend_t.
 *
 *  \param c    chanend to convert.
 *
 *  \return     the streaming_chanend_t
 */
inline streaming_chanend_t s_chanend_convert(chanend c)
{
  return (streaming_chanend_t)c;
}

/** Allocate a single chanend.
 *
 *  This function allocates a hardware channel end.
 *  If there are no channel ends available the function returns 0.
 *  When the channel end is no longer required, chanend_free() must be called
 *  to deallocate it.
 *
 *  \param c    chanend or zero if none are available.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *\*c* argument.
 */
inline chanend chanend_alloc()
{
  return (chanend)s_chanend_alloc();
}

/** Deallocate a single chanend.
 *
 *  This function frees the hardware chanend.
 *  The last transfer on the chanend must have been a CT_END token.
 *
 *  \param c    chanend to free.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chanend,
 *                                    an input/output is pending,
 *                                    or it has not received/sent a CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chanend.
 *  \exception  ET_LOAD_STORE         invalid *\*c* argument.
 */
inline void chanend_free(chanend c)
{
  s_chanend_free((streaming_chanend_t)c);
}

/** Set the destination of a chanend
 *
 *  \param c    chanend to set.
 *
 *  \param dst  Destination chanend.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chanend.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chanend.
*/
inline void chanend_set_dest(chanend c, chanend dst)
{
  s_chanend_set_dest((streaming_chanend_t)c, (streaming_chanend_t)dst);
}

/** Convert a streaming_chanend_t to a chanend.
 *
 *  **streaming_chanend_t must have completed any transaction with an end-token
 *  handshake before being converting into a chanend.**
 *
 *  \param c    streaming_chanend_t to convert.
 *
 *  \return     the chanend
 */
inline chanend chanend_convert(streaming_chanend_t c)
{
  return (chanend)c;
}

/** Setup select events on a chan-end.
 *
 *  Configures a chan-end to trigger select events when data is ready.
 *  It is used in combination with select_wait() et al functions,
 *  returning the enum_id when the event is triggered.
 *
 *  Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c        The chan-end to setup the select event on
 *  \param enum_id  The value to be returned by select_wait() et al when the
 *                  chan-end event is triggered.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
*/
inline void chanend_setup_select(chanend c, uint32_t enum_id)
{
  _resource_setup_select(c, enum_id);
}

/** Setup select events on a chan-end where the events are handled by a function.
 *
 *  Same as chanend_setup_select() except that a callback function is used
 *  rather than the event being passed back to the select_wait() et al functions.
 *
 *  Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c      The chan-end to setup the select event on
 *  \param data   The value to be passed to the select_callback_t function
 *  \param func   The select_callback_t function to handle the event
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline void chanend_setup_select_callback(chanend c, void *data, select_callback_t func)
{
  _resource_setup_select_callback(c, data, func);
}

/** Setup interrupt events on a chan-end .
 *
 *  Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c     The chan-end to setup the events on
 *  \param data  The value to be passed to the interrupt_callback_t function
 *  \param func  The interrupt_callback_t function to handle events
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline void chanend_setup_interrupt_callback(chanend c, void *data,
                                             interrupt_callback_t func)
{
  _resource_setup_interrupt_callback(c, data, func);
}

/** Enable select & interrupt events on a chan-end.
 *
 *  Prior to enabling, chanend_setup_select(), chanend_setup_select_callback() or
 *  chanend_setup_interrupt_callback() must have been called.
 *  Events can be temporarily disabled and re-enabled using chanend_disable_trigger()
 *  and chanend_enable_trigger().
 *  When the event fires, the value must be read from the chan-end to clear the event.
 *
 *  \param c    The chan-end to enable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void chanend_enable_trigger(chanend c)
{
  _resource_enable_trigger(c);
}

/** Disable select & interrupt events for a given chan-end.
 *
 *  This function prevents any further events being triggered by a given chan-end.
 *
 *  \param c    The chan-end to disable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void chanend_disable_trigger(chanend c)
{
  _resource_disable_trigger(c);
}

#endif // !defined(__XC__)

#endif // __xcore_c_chanend_h__
