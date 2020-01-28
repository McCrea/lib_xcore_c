// Copyright (c) 2016, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief Low level channel end API
 */

#include <stdint.h>
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_chan_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>

/** \typedef streaming_chanend_t
 *  \brief Opaque channel end type for use in C/C++ code.
 *
 *  \attention Users must not access its raw underlying type.
 */
typedef resource_t chanend_t;

/** \typedef streaming_chanend_t
 *  \brief Opaque streaming channel end type for use in C/C++ code.
 *
 *  \attention Users must not access its raw underlying type.
 */
typedef resource_t streaming_chanend_t;

/** \brief Allocate a single streaming_chanend_t.
 * 
 *  If there are no channel ends available the function returns 0.
 *  \note When the channel end is no longer required, s_chanend_free() should be called
 *  to deallocate it.
 *
 *  \return     chanend (0 if none are available)
 */
_XCORE_EXFUN
inline streaming_chanend_t s_chanend_alloc(void)
{
  return __xcore_s_chanend_alloc();
}

/** \brief Deallocate a single streaming_chanend_t.
 *
 *  \attention The last transfer on the streaming_chanend_t must have been a CT_END token.
 *
 *  \param c    streaming_chanend_t to free.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   \a c is not an allocated streaming_chanend_t,
 *                                    an input/output is pending,
 *                                    or it has not received/sent a CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the streaming_chanend_t.
 */
_XCORE_EXFUN
inline void s_chanend_free(streaming_chanend_t c)
{
  __xcore_s_chanend_free(c);
}

/** \brief Set the destination of a streaming_chanend_t
 *
 *  \param c    streaming_chanend_t to set.
 *  \param dst  Destination streaming_chanend_t.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated streaming_chanend_t.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the streaming_chanend_t.
*/
_XCORE_EXFUN
inline void s_chanend_set_dest(streaming_chanend_t c, streaming_chanend_t dst)
{
  __xcore_s_chanend_set_dest(c, dst);
}

/** \brief Convert a chanend to a streaming_chanend_t.
 *
 *  \note A chanend is always in a safe state for converting into a streaming_chanend_t.
 *
 *  \param c    chanend to convert.
 *  \return     the streaming_chanend_t
 */
_XCORE_EXFUN
inline streaming_chanend_t s_chanend_convert(chanend_t c)
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
_XCORE_EXFUN
inline chanend_t chanend_alloc()
{
  return (chanend_t)s_chanend_alloc();
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
_XCORE_EXFUN
inline void chanend_free(chanend_t c)
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
_XCORE_EXFUN
inline void chanend_set_dest(chanend_t c, chanend_t dst)
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
_XCORE_EXFUN
inline chanend_t chanend_convert(streaming_chanend_t c)
{
  return (chanend_t)c;
}
