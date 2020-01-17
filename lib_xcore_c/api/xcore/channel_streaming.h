// Copyright (c) 2016, XMOS Ltd, All rights reserved

/** \file
 *  \brief Streaming channel API
 */

#ifndef __xcore_c_channel_streaming_h__
#define __xcore_c_channel_streaming_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <stddef.h>
#include <xcore/_support/xcore_c_chan_impl.h>

/** \typedef streaming_channel_t
 *  \brief Helper type for passing around both ends of a streaming channel.
 */
typedef struct streaming_channel_t {
  streaming_chanend_t end_a;
  streaming_chanend_t end_b;
} streaming_channel_t;

/** \brief Allocate a streaming channel by allocating two hardware chan-ends and joining them.
 *
 *  If there are not enough chan-ends available the function returns a
 *  streaming_channel_t with both fields set to 0.
 *  \note When the streaming_channel_t is no longer required, s_chan_free() should be
 *  called to deallocate it.
 *
 *  \attention The chan-ends must be accessed on the same tile
 *
 *  \return    streaming_channel_t variable holding the two initialised and
 *             joined chan-ends or 0s.
 */
inline streaming_channel_t s_chan_alloc()
{
  streaming_channel_t c;

  if ((c.end_a = _s_chanend_alloc()))
  {
    if ((c.end_b = _s_chanend_alloc()))
    {
      // exception safe calls to _s_chanend_set_dest()
      _s_chanend_set_dest(c.end_a, c.end_b);
      _s_chanend_set_dest(c.end_b, c.end_a);
    }
    else
    {
      _s_chanend_free(c.end_a);
      c.end_a = 0;
      c.end_b = 0;
    }
  }
  else
  {
    c.end_a = 0;
    c.end_b = 0;
  }

  return c;
}

/** \brief Deallocate a streaming_channel_t by freeing its two hardware chan-ends.
 *
 *  \param c    streaming_channel_t to free.
 *
 *  \exception  ET_LINK_ERROR         a chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated channel,
 *                                    or an input/output is pending.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the channel.
 */
inline void s_chan_free(streaming_channel_t c)
{
  _s_chan_out_ct_end(c.end_a);
  _s_chan_out_ct_end(c.end_b);
  _s_chan_check_ct_end(c.end_a);
  _s_chan_check_ct_end(c.end_b);
  _s_chanend_free(c.end_a);
  _s_chanend_free(c.end_b);
}

/** \brief Output a word over a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \param data The word to be output
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void s_chan_out_word(streaming_chanend_t c, uint32_t data)
{
  _s_chan_out_word(c, data);
}

/** \brief Output an byte over a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \param data The byte to be output
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void s_chan_out_byte(streaming_chanend_t c, uint8_t data)
{
  _s_chan_out_byte(c, data);
}

/** \brief Output a block of data over a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \param buf  A pointer to the buffer containing the data to send
 *  \param n    The number of words to send
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline void s_chan_out_buf_word(streaming_chanend_t c, const uint32_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    _s_chan_out_word(c, buf[i]);
  }
}

/** \brief Output a block of data over a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \param buf  A pointer to the buffer containing the data to send
 *  \param n    The number of bytes to send
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline void s_chan_out_buf_byte(streaming_chanend_t c, const uint8_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    _s_chan_out_byte(c, buf[i]);
  }
}

/** \brief Input a word from a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \return     The word read from the channel
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline uint32_t s_chan_in_word(streaming_chanend_t c)
{
  return _s_chan_in_word(c);
}

/** \brief Input a byte from a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \return     The byte read from the channel
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline uint8_t s_chan_in_byte(streaming_chanend_t c)
{
  return _s_chan_in_byte(c);
}

/** \brief Input a block of data from a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \param buf  A pointer to the memory region to fill
 *  \param n    The number of words to receive
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline void s_chan_in_buf_word(streaming_chanend_t c, uint32_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = _s_chan_in_word(c);
  }
}

/** \brief Input a block of data from a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \param buf  A pointer to the memory region to fill
 *  \param n    The number of bytes to receive
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *buf[]* argument.
 */
inline void s_chan_in_buf_byte(streaming_chanend_t c, uint8_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = _s_chan_in_byte(c);
  }
}

/** \brief Output a control token onto a streaming_channel_t.
 *
 *  \param c    The streaming chan-end
 *  \param ct   Control token to be output. Legal control tokens that can be
 *              used are 0 or any value in the range 3..191 inclusive.
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set
 *                                    or token is reserverd.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void s_chan_out_ct(streaming_chanend_t c, uint8_t ct)
{
  _s_chan_out_ct(c, ct);
}

/** \brief Output a CT_END control token onto a streaming_channel_t.
 *
 *  Outputting a CT_END control token informs the communication network and the
 *  other chan-end that the current transaction has completed. Thus freeing the
 *  communication network for other channels to use.
 *
 *  \note The streaming_channel_t remains allocated, awaiting another transaction or
 *  deallocation.
 *
 *  \param c    The streaming chan-end
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void s_chan_out_ct_end(streaming_chanend_t c)
{
  _s_chan_out_ct_end(c);
}

/** \brief Check that a specific control token on a streaming_channel_t.
 *
 *  This function blocks until a token is available on the \a c. If
 *  the available token is a control token and has the value \a ct, then the
 *  token is input and discarded. Otherwise an exception is raised.
 *
 *  \param c    The streaming chan-end
 *  \param ct   Control token that is expected on the streaming_channel_t.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or does not contain expected token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void s_chan_check_ct(streaming_chanend_t c, uint8_t ct)
{
  _s_chan_check_ct(c, ct);
}

/** \brief Check for a CT_END token on a streaming_channel_t.
 *
 *  This function blocks until a token is available on the \a c. If
 *  the available token is a CT_END token the token is input and discarded.
 *  Otherwise an exception is raised.
 *
 *  \param c    The streaming chan-end
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or does not contain CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline void s_chan_check_ct_end(streaming_chanend_t c)
{
  _s_chan_check_ct_end(c);
}

#endif // !defined(__XC__)

#endif // __xcore_c_channel_streaming_h__
