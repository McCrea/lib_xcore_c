// Copyright (c) 2016, XMOS Ltd, All rights reserved

/** \file
 *  \brief Low level channel end API
 */

#ifndef __xcore_c_chanend_h__
#define __xcore_c_chanend_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <xcore/_support/xcore_c_chan_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>

#ifdef __DOXYGEN__
// Copy typedefs from xccompat.h for use by doxygen
/** \typedef chanend
 *  \brief Opaque channel type for use in C/C++ code.
 *
 *  It enables a xC function prototyped as taking a parameter of type chanend to
 *  be called from C and vice versa.
 *
 *  \attention Users must not access its raw underlying type.
 */
typedef unsigned chanend;

/** \typedef streaming_chanend_t
 *  \brief Opaque streaming channel type for use in C/C++ code.
 *
 * It enables a xC function prototyped as taking a parameter of type
 * streaming_chanend_t to be called from C and vice versa.
 *
 *  \attention Users must not access its raw underlying type.
 */
typedef unsigned streaming_chanend_t;
#endif

/** \brief Allocate a single streaming_chanend_t.
 * 
 *  If there are no channel ends available the function returns 0.
 *  \note When the channel end is no longer required, s_chanend_free() should be called
 *  to deallocate it.
 *
 *  \return     chanend (0 if none are available)
 */
inline streaming_chanend_t s_chanend_alloc(void)
{
  return _s_chanend_alloc();
}

/** \brief Deallocate a single streaming_chanend_t.
 *
 *  \attention The last transfer on the streaming_chanend_t must have been a CT_END token.
 *
 *  \param c    streaming_chanend_t to free.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   \a c isnot an allocated streaming_chanend_t,
 *                                    an input/output is pending,
 *                                    or it has not received/sent a CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the streaming_chanend_t.
 */
inline void s_chanend_free(streaming_chanend_t c)
{
  _s_chanend_free(c);
}

/** \brief Set the destination of a streaming_chanend_t
 *
 *  \param c    streaming_chanend_t to set.
 *  \param dst  Destination streaming_chanend_t.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated streaming_chanend_t.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the streaming_chanend_t.
*/
inline void s_chanend_set_dest(streaming_chanend_t c, streaming_chanend_t dst)
{
  _s_chanend_set_dest(c, dst);
}

/** \brief Convert a chanend to a streaming_chanend_t.
 *
 *  \note A chanend is always in a safe state for converting into a streaming_chanend_t.
 *
 *  \param c    chanend to convert.
 *  \return     the streaming_chanend_t
 */
inline streaming_chanend_t s_chanend_convert(chanend c)
{
  return (streaming_chanend_t)c;
}

/** \brief Allocate a single chanend.
 *
 *  If there are no channel ends available the function returns 0.
 *  \note When the channel end is no longer required, chanend_free() should be called
 *  to deallocate it.
 *
 *  \return    Allocated chanend (0 if none are available)
 */
inline chanend chanend_alloc()
{
  return (chanend)s_chanend_alloc();
}

/** \brief Deallocate a single chanend.
 * 
 *  \attention The last transfer on the chanend must have been a CT_END token.
 *
 *  \param c    chanend to free.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chanend,
 *                                    an input/output is pending,
 *                                    or it has not received/sent a CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chanend.
 */
inline void chanend_free(chanend c)
{
  s_chanend_free((streaming_chanend_t)c);
}

/** \brief Set the destination of a chanend
 *
 *  \param c    chanend to set.
 *  \param dst  Destination chanend.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chanend.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chanend.
*/
inline void chanend_set_dest(chanend c, chanend dst)
{
  s_chanend_set_dest((streaming_chanend_t)c, (streaming_chanend_t)dst);
}

/** \brief Convert a streaming_chanend_t to a chanend.
 * 
 *  \attention streaming_chanend_t must have completed any transaction with an end-token
 *  handshake before being converting into a chanend.
 *
 *  \param c    streaming_chanend_t to convert.
 *  \return     the chanend
 */
inline chanend chanend_convert(streaming_chanend_t c)
{
  return (chanend)c;
}

/** \brief Setup select events on a chan-end.
 * 
 *  Configures a chan-end to trigger select events when data is ready.
 *  It is used in combination with select_wait() et al functions,
 *  returning the enum_id when the event is triggered.
 *
 *  \note Once the event is set up you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c        The chan-end to setup the select event on
 *  \param enum_id  The value to be returned by select_wait() et al when the
 *                  chan-end event is triggered.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set \a enum_id.
*/
inline void chanend_setup_select(chanend c, uint32_t enum_id)
{
  _resource_setup_select(c, enum_id);
}

/** \brief Setup select events on a chan-end where the events are handled by a function.
 *
 *  Same as chanend_setup_select() except that a callback function is used
 *  rather than the event being passed back to the select_wait() et al functions.
 *
 *  \note Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c          The chan-end to setup the select event on
 *  \param[in] data   The value to be passed to the select_callback_t function
 *  \param func       The select_callback_t function to handle the event

 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in \a data.
 */
inline void chanend_setup_select_callback(chanend c, void *data, select_callback_t func)
{
  _resource_setup_select_callback(c, data, func);
}

/** \brief Setup interrupt events on a chan-end.
 * 
 *  \note Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c         The chan-end to setup the events on
 *  \param[in] data  The value to be passed to the interrupt_callback_t function
 *  \param func      The interrupt_callback_t function to handle events
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in \a data.
 */
inline void chanend_setup_interrupt_callback(chanend c, void *data,
                                             interrupt_callback_t func)
{
  _resource_setup_interrupt_callback(c, data, func);
}

/** \brief Enable select & interrupt events on a chan-end.
 *
 *  Prior to enabling, chanend_setup_select(), chanend_setup_select_callback() or
 *  chanend_setup_interrupt_callback() must have been called.
 *  Events can be temporarily disabled and re-enabled using chanend_disable_trigger()
 *  and chanend_enable_trigger().
 *  \note When the event fires, the value must be read from the chan-end to clear the event.
 *
 *  \param c    The chan-end to enable events on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void chanend_enable_trigger(chanend c)
{
  _resource_enable_trigger(c);
}

/** \brief Disable select & interrupt events for a given chan-end.
 *
 *  This function prevents any further events being triggered by a given chan-end.
 *
 *  \param c    The chan-end to disable events on
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
