// Copyright (c) 2016, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief Low level channel end API
 */

#include <stdint.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_chan_impl.h>
#include <xcore/_support/xcore_resource_impl.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \typedef chanend_t
 *  \brief Opaque channel end type for use in C/C++ code.
 *
 *  \attention Users must not access its raw underlying type.
 */
typedef resource_t chanend_t;


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
  return __xcore_chanend_alloc();
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
  __xcore_chanend_free(c);
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
  __xcore_chanend_set_dest(c, dst);
}

// New
_XCORE_EXFUN
inline chanend_t chanend_get_dest(chanend_t c)
{
  return __xcore_chanend_get_dest(c);
}

_XCORE_EXFUN
inline void chanend_out_byte(chanend_t c, char b)
{
  __xcore_chanend_out_byte(c, b);
}

_XCORE_EXFUN
inline void chanend_out_word(chanend_t c, uint32_t w)
{
  __xcore_chanend_out_word(c, w);
}

_XCORE_EXFUN
inline void chanend_out_control_token(chanend_t c, char ct)
{
  __xcore_chanend_out_ct(c, ct);
}

_XCORE_EXFUN
inline char chanend_in_byte(chanend_t c)
{
  return __xcore_chanend_in_byte(c);
}

_XCORE_EXFUN
inline uint32_t chanend_in_word(chanend_t c)
{
  return __xcore_chanend_in_word(c);
}

_XCORE_EXFUN
inline char chanend_in_control_token(chanend_t c)
{
  return __xcore_chanend_in_ct(c);
}

_XCORE_EXFUN
inline void chanend_check_control_token(chanend_t c, const char ct)
{
  __xcore_chanend_check_ct(c, ct);
}

_XCORE_EXFUN
inline _Bool chanend_test_control_token_next_byte(chanend_t c)
{
  return __xcore_chanend_test_control_token(c);
}

_XCORE_EXFUN
inline int chanend_test_control_token_next_word(chanend_t c)
{
  return __xcore_chanend_test_control_token_word(c);
}

_XCORE_EXFUN
inline _Bool chanend_test_dest_local(chanend_t c)
{
  return __xcore_chanend_test_dest_local(c);
}

_XCORE_EXFUN
inline void chanend_set_network(chanend_t c, uint32_t net)
{
  __xcore_channend_set_network(c, net);
}

_XCORE_EXFUN
inline unsigned chanend_get_network(chanend_t c)
{
  return __xcore_channend_get_network(c);
}

_XCORE_EXFUN
inline void chanend_out_end_token(resource_t c)
{
  __xcore_chanend_out_ct(c, XS1_CT_END);
}

_XCORE_EXFUN 
inline void chanend_check_end_token(resource_t c)
{
  __xcore_chanend_check_ct(c, XS1_CT_END);
}

#ifdef __cplusplus
}
#endif

