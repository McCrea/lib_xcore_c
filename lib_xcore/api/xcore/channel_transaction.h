// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief API for transaction-based channel communications
 */

#include <stdint.h>
#include <stddef.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_chan_impl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  __xcore_streaming_chanend_t __c;
  unsigned __last_out;
} __xcore_transacting_chanend_t;

/** \brief An opaque type for handling transactions
 *
 *  Users must not access its raw underlying type.
 */
typedef __xcore_transacting_chanend_t transacting_chanend_t;

// Manage direction changes.
// As specified in the Tools Development Guide, the last_out state is managed
// to control when CT_END tokens are sent or expected.
_XCORE_EXFUN
inline void __xcore_t_chan_change_to_input(transacting_chanend_t *tc)
{
  if (tc->__last_out) {
    chanend_out_end_token(tc->__c);
    tc->__last_out = 0;
  }
}

_XCORE_EXFUN
inline void __xcore_t_chan_change_to_output(transacting_chanend_t *tc)
{
  if (!tc->__last_out) {
    chanend_check_end_token(tc->__c);
    tc->__last_out = 1;
  }
}

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
_XCORE_EXFUN
inline transacting_chanend_t chan_init_transaction_master(chanend_t c)
{
  chanend_out_end_token(c);

  transacting_chanend_t tc;
  tc.__last_out = 0;
  tc.__c = c;

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
_XCORE_EXFUN
inline transacting_chanend_t chan_init_transaction_slave(chanend_t c)
{
  chanend_check_end_token(c);

  transacting_chanend_t tc;
  tc.__last_out = 1;
  tc.__c = c;

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
_XCORE_EXFUN
inline chanend_t chan_complete_transaction(transacting_chanend_t tc)
{
  if (tc.__last_out)
  {
    chanend_out_end_token(tc.__c);
    chanend_check_end_token(tc.__c);
  }
  else
  {
    chanend_check_end_token(tc.__c);
    chanend_out_end_token(tc.__c);
  }

  return tc.__c;
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
_XCORE_EXFUN
inline void t_chan_out_word(transacting_chanend_t *tc, uint32_t data)
{
  __xcore_t_chan_change_to_output(tc);
  chanend_out_word(tc->__c, data);
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
_XCORE_EXFUN
inline void t_chan_out_byte(transacting_chanend_t *tc, uint8_t data)
{
  __xcore_t_chan_change_to_output(tc);
  chanend_out_byte(tc->__c, data);
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
_XCORE_EXFUN
inline void t_chan_out_buf_word(transacting_chanend_t *tc, const uint32_t buf[], size_t n)
{
  __xcore_t_chan_change_to_output(tc);
  for (size_t i = 0; i < n; i++)
  {
    chanend_out_word(tc->__c, buf[i]);
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
_XCORE_EXFUN
inline void t_chan_out_buf_byte(transacting_chanend_t *tc, const uint8_t buf[], size_t n)
{
  __xcore_t_chan_change_to_output(tc);
  for (size_t i = 0; i < n; i++)
  {
    chanend_out_byte(tc->__c, buf[i]);
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
_XCORE_EXFUN
inline uint32_t t_chan_in_word(transacting_chanend_t *tc)
{
  __xcore_t_chan_change_to_input(tc);
  return chanend_in_word(tc->__c);
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
_XCORE_EXFUN
inline uint8_t t_chan_in_byte(transacting_chanend_t *tc)
{
  __xcore_t_chan_change_to_input(tc);
  return chanend_in_byte(tc->__c);
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
_XCORE_EXFUN
inline void t_chan_in_buf_word(transacting_chanend_t *tc, uint32_t buf[], size_t n)
{
  __xcore_t_chan_change_to_input(tc);
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = chanend_in_word(tc->__c);
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
_XCORE_EXFUN
inline void t_chan_in_buf_byte(transacting_chanend_t *tc, uint8_t buf[], size_t n)
{
  __xcore_t_chan_change_to_input(tc);
  for (size_t i = 0; i < n; i++)
  {
    buf[i] = chanend_in_byte(tc->__c);
  }
}

#ifdef __cplusplus
}
#endif

