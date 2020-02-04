// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief Streaming channel API
 */

#include <stdint.h>
#include <stddef.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_chan_impl.h>
#include <xcore/chanend.h>

/** \typedef streaming_channel_t
 *  \brief Helper type for passing around both ends of a streaming channel.
 */
typedef struct streaming_channel_t {
  chanend_t end_a;
  chanend_t end_b;
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
_XCORE_EXFUN
inline streaming_channel_t s_chan_alloc()
{
  streaming_channel_t c;

  if ((c.end_a = chanend_alloc()))
  {
    if ((c.end_b = chanend_alloc()))
    {
      // exception safe calls to __xcore_s_chanend_set_dest()
      chanend_set_dest(c.end_a, c.end_b);
      chanend_set_dest(c.end_b, c.end_a);
    }
    else
    {
      chanend_free(c.end_a);
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
_XCORE_EXFUN
inline void s_chan_free(streaming_channel_t c)
{
  chanend_out_end_token(c.end_a);
  chanend_out_end_token(c.end_b);
  chanend_check_end_token(c.end_a);
  chanend_check_end_token(c.end_b);
  chanend_free(c.end_a);
  chanend_free(c.end_b);
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
_XCORE_EXFUN
inline void s_chan_out_word(chanend_t c, uint32_t data)
{
  chanend_out_word(c, data);
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
_XCORE_EXFUN
inline void s_chan_out_byte(chanend_t c, uint8_t data)
{
  chanend_out_byte(c, data);
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
_XCORE_EXFUN
inline void s_chan_out_buf_word(chanend_t c, const uint32_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    chanend_out_word(c, buf[i]);
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
_XCORE_EXFUN
inline void s_chan_out_buf_byte(chanend_t c, const uint8_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    chanend_out_byte(c, buf[i]);
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
_XCORE_EXFUN
inline uint32_t s_chan_in_word(chanend_t c)
{
  return chanend_in_word(c);
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
_XCORE_EXFUN
inline uint8_t s_chan_in_byte(chanend_t c)
{
  return chanend_in_byte(c);
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
_XCORE_EXFUN
inline void s_chan_in_buf_word(chanend_t c, uint32_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = chanend_in_word(c);
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
_XCORE_EXFUN
inline void s_chan_in_buf_byte(chanend_t c, uint8_t buf[], size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = chanend_in_byte(c);
  }
}

