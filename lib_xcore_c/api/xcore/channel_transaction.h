// Copyright (c) 2016, XMOS Ltd, All rights reserved

/** \file
 *  \brief API for transaction-based channel communications
 */

#ifndef __xcore_c_channel_transaction_h__
#define __xcore_c_channel_transaction_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <stddef.h>
#include <xcore/_support/xcore_c_chan_impl.h>

/** \brief Start a transaction (master).
 *
 *  This initiates a transaction on a channel.
 *
 *  A transacting_chanend_t is used to temporarily open a transaction
 *  route through a channel.
 *  During the transaction, you can use transaction channel operations for
 *  increased efficiency.
 *  You can create a transacting chanend from a normal *chanend* using
 *  chan_init_transaction_master() and chan_init_transaction_slave().
 *
 *  This called must be matched by a call to chan_init_transaction_slave()
 *  on the other end of the channel.
 *
 *  \note A transaction must be closed with chan_complete_transaction().
 *
 *  \param c    chan-end to initialize the transaction on. 
 *  \return     the intialized master transacting_chanend_t
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline transacting_chanend_t chan_init_transaction_master(chanend c)
{
  _s_chan_out_ct_end(c);

  transacting_chanend_t tc;
  tc.last_out = 0;
  tc.c = (streaming_chanend_t)c;

  return tc;
}

/** \brief Start a transaction (slave).
 *
 *  This call must be matched by a call to chan_init_transaction_master()
 *  on the other end of the channel.
 *
 *  \note A transaction must be closed with chan_complete_transaction().
 *
 *  \warning The original channed \a c must not be used until the transaction is closed.
 *
 *  \param c    chan-end to initialize the transaction on. chanend is invalidated
 *  \return     the intialized slave transacting_chanend_t
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or does not contain CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline transacting_chanend_t chan_init_transaction_slave(chanend c)
{
  _s_chan_check_ct_end(c);

  transacting_chanend_t tc;
  tc.last_out = 1;
  tc.c = (streaming_chanend_t)c;

  return tc;
}

/** \brief Completes a transaction. 
 *  After this call the route between the
 *  two ends of the channel is freed allowing other channels to use the
 *  communication network.
 *
 *  Whilst the transacting_chanend_t is now invalid, the channel remains allocated,
 *  awaiting another transaction or deallocation.
 *
 *  \note This call must be accompanied by a call to chan_complete_transaction() on
 *  the other end of the channel.
 *
 *  \param tc  Transacting chan-end to close.
 *  \return    The original chan-end which is valid once again.
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline chanend chan_complete_transaction(transacting_chanend_t tc)
{
  if (tc.last_out)
  {
    _s_chan_out_ct_end(tc.c);
    _s_chan_check_ct_end(tc.c);
  }
  else
  {
    _s_chan_check_ct_end(tc.c);
    _s_chan_out_ct_end(tc.c);
  }

  return (chanend)tc.c;
}

/** \brief Output a word over a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \param              data Word to be output
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc argument.
 */
inline void t_chan_out_word(transacting_chanend_t *tc, uint32_t data)
{
  _t_chan_change_to_output(tc);
  _s_chan_out_word(tc->c, data);
}

/** \brief Output an byte over a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \param data         Byte to be output
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc argument.
 */
inline void t_chan_out_byte(transacting_chanend_t *tc, uint8_t data)
{
  _t_chan_change_to_output(tc);
  _s_chan_out_byte(tc->c, data);
}

/** \brief Output a block of data over a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \param[in] buf      Pointer to the buffer containing the data to send
 *  \param n            Number of words to send
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc or \a buf[] argument.
 */
inline void t_chan_out_buf_word(transacting_chanend_t *tc, const uint32_t buf[], size_t n)
{
  _t_chan_change_to_output(tc);
  for (size_t i = 0; i < n; i++)
  {
    _s_chan_out_word(tc->c, buf[i]);
  }
}

/** \brief Output a block of data over a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \param[in] buf      Pointer to the buffer containing the data to send
 *  \param n            Number of bytes to send
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc or \a buf[] argument.
 */
inline void t_chan_out_buf_byte(transacting_chanend_t *tc, const uint8_t buf[], size_t n)
{
  _t_chan_change_to_output(tc);
  for (size_t i = 0; i < n; i++)
  {
    _s_chan_out_byte(tc->c, buf[i]);
  }
}

/** \brief Input a word from a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \return Word read from \a tc
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc argument.
 */
inline uint32_t t_chan_in_word(transacting_chanend_t *tc)
{
  _t_chan_change_to_input(tc);
  return _s_chan_in_word(tc->c);
}

/** \brief Input a byte from a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \return Byte read from \a tc
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc or \a data argument.
 */
inline uint8_t t_chan_in_byte(transacting_chanend_t *tc)
{
  _t_chan_change_to_input(tc);
  return _s_chan_in_byte(tc->c);
}

/** \brief Input a block of data from a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \param[in] buf      Pointer to the memory region to fill
 *  \param n            The number of words to receive
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc or \a buf[] argument.
 */
inline void t_chan_in_buf_word(transacting_chanend_t *tc, uint32_t buf[], size_t n)
{
  _t_chan_change_to_input(tc);
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = _s_chan_in_word(tc->c);
  }
}

/** \brief Input a block of data from a transacting chan-end.
 *
 *  \param[in,out] tc   Transacting chan-end
 *  \param[in] buf      Pointer to the memory region to fill
 *  \param n            The number of bytes to receive
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid \a tc or \a buf[] argument.
 */
inline void t_chan_in_buf_byte(transacting_chanend_t *tc, uint8_t buf[], size_t n)
{
  _t_chan_change_to_input(tc);
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = _s_chan_in_byte(tc->c);
  }
}

#endif // !defined(__XC__)

#endif // __xcore_c_channel_transaction_h__
