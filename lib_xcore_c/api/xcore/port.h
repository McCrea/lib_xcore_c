// Copyright (c) 2016, XMOS Ltd, All rights reserved

/** \file
 *  \brief API for IO using ports
 */

#ifndef __xcore_c_port_h__
#define __xcore_c_port_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <stddef.h>
#include <xcore/_support/xcore_c_port_impl.h>
#include <xcore/_support/xcore_c_resource_impl.h>

#include <xcore/port_id.h>

/** \brief Enumeration to declare how the port was set up */
typedef enum {
  PORT_UNBUFFERED = 0,
  PORT_BUFFERED = 1,
} port_type_t;

/** \brief Enables a port.
 *
 *  Either this function or port_start_buffered() must be called once for each
 *  variable of type \c port before use. port_disable() must be called afterwards.
 *
 *  The port's state is set to: input, unbuffered, inout_data, no_invert,
 *  rising_edge, master, no_ready, no triggers, clocked by XS1_CLKBLK_REF.
 *
 *  \param p  The port to enable
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *
 * \see port_enable_buffered
 */
inline void port_enable(port_id_t p)
{
  _port_enable(p);
}

/** \brief Reset a port.
 *
 *  Clears a ports settings back to the default state at port_alloc().
 *
 *  \param p  The port to be reset
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_reset(port_id_t p)
{
  _port_reset(p);
}

/** \brief Allocates a port to buffer and serialise/deserialise data.
 *
 *  Either this function or port_enable() must be called once for each port_id_t
 *  before it is used.
 *
 *  \param p               Value that identifies which port to drive
 *  \param transfer_width  Number of bits to serialise; must be 1, 4, 8, or 32.
 *                         The number of bits must be >= to the physical port
 *                         width.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
 *                                    or is not legal width for the port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 * 
 *  \see port_enable
 */
inline void port_start_buffered(port_id_t p, size_t transfer_width)
{
  _port_enable(p);
  _port_set_buffered(p);
  _port_set_transfer_width(p, transfer_width);
}

/** \brief Disable a port
 *
 *  Disables the port so it is no longer ready for use. It must be re-enabled if it is used again.
 *  \param p  The port to be disabled
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_disable(port_id_t p)
{
  _port_free(p);
}

/** \brief Change the transfer width of a port.
 *
 *  The default transfer width is the same as the physical port width.
 *
 *  \attention A port must have been set to buffered if the width is different from the
 *             physical port width
 *
 *  \param p               The port to change the transfer width of
 *  \param transfer_width  Number of bits to serialise; must be 1, 4, 8, or 32.
 *                         The number of bits must be >= to the physical port
 *                         width.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
 *                                    or is not legal width for the port,
 *                                    or the port is unbuffered.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_transfer_width(port_id_t p, size_t transfer_width)
{
  _port_set_transfer_width(p, transfer_width);
}

/** \brief Sets a port to be buffered.
 *
 *  Configures a port into buffered mode where it can automatically serialise or
 *  deserialise data.
 *
 *  \param p  The port to set as buffered
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_buffered(port_id_t p)
{
  _port_set_buffered(p);
}

/** \brief Sets a port to be unbuffered (default state).
 *
 *  Configures a port into unbuffered mode. Note that before this is called, a
 *  a port needs to have its transfer width equal to the port width and be
 *  configured as a master port.
 *
 *  \param p  The port to set as unbuffered
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_unbuffered(port_id_t p)
{
  _port_set_unbuffered(p);
}

/** \brief Set the clock clocking a port.
 *
 *  Changes the clock used for a port's control functions.
 *  The default clock is XS1_CLKBLK_REF.
 *
 *  \param p    Port whose clock is being changed.
 *  \param clk  Clock to attach to the port.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock, or clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_clock(port_id_t p, xclock clk)
{
  _port_set_clock(p, clk);
}

/** \brief Set a port drive out the data value (default state).
 *
 *  \param p  Port to change the mode of.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_inout_data(port_id_t p)
{
  _port_set_inout_data(p);
}

/** \brief Set a port to drive out its clocking signal.
 *
 *  Configures the port to drive the clock signal instead of its
 *  own data values. The clock signal that is driven out is configured using the
 *  port_set_clock() function.
 *
 *  \param p  Port to change the mode of.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_out_clock(port_id_t p)
{
  _port_set_out_clock(p);
}

/** \brief Set a port to drive out the ready signal of another port.
 *
 *  Configures the port to drive the ready signal of another port
 *  instead of its own data values.
 *
 *  \param p             Port to change the mode of. This must be a 1-bit
 *                       port or this function will trap.
 *  \param ready_source  The port whose ready signal will be output.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   \li not a valid port \li \a p not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_out_ready(port_id_t p, port_id_t ready_source)
{
  _port_set_out_ready(p, ready_source);
}

/** \brief Set the port to invert its data.
 *
 *  Configures a port to invert the data on the pin. This can be
 *  reverted by calling port_set_no_invert().
 *
 *  \param p  Port to set its data to be inverted. This must be a 1-bit port
 *            or a trap will be raised.
 *
 *  \exception  ET_ILLEGAL_RESOURCE  \li not a valid port \li p not a one bit port.
 *  \exception  ET_RESOURCE_DEP      another core is actively changing the port.
 */
inline void port_set_invert(port_id_t p)
{
  _port_set_invert(p);
}

/** \brief Set the port to not invert its data (default state).
 *
 *  Configures a port to not invert the data on the pin.
 *
 *  \param p  Port to set the data to not be inverted.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_no_invert(port_id_t p)
{
  _port_set_no_invert(p);
}

/** \brief Set the port to sample on the falling edge.
 *
 *  The default is for a port to sample data on the rising edge of the clock.
 *  This function changes the port to sample on the falling edge instead.
 *  This change can be reverted by calling port_set_sample_rising_edge().
 *
 *  \param p  Port to change its sampling edge.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_sample_falling_edge(port_id_t p)
{
  _port_set_sample_falling_edge(p);
}

/** \brief Set the port to sample on the rising edge (default state).
 *
 *  Restores a port to sampling data on the rising edge of the clock.
 *
 *  \param p  Port to change its sampling edge.
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_sample_rising_edge(port_id_t p)
{
  _port_set_sample_rising_edge(p);
}

/** \brief Set the port to master mode (default state).
 *
 *  Configures a port to be a master. This is only relevant when
 *  using ready signals (port_set_ready_strobed() / port_set_ready_handshake()).
 *
 *  \param p  Port to set as a master
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *
 *  \note It is highly recommended to use the \c port_protocol_* functions to put a
 *        port into its desired mode as the order of operations is critical.
 *  \see xcore/port_protocol.h
 */
inline void port_set_master(port_id_t p)
{
  _port_set_master(p);
}

/** \brief Set the port to slave mode.
 *
 *  Configures a port to be a master. This is only relevant when
 *  using a ready strobe (port_set_ready_strobed())
 *
 *  \attention the port must be set to use a ready strobe, otherwise this function
 *             will raise an exception.
 *
 *  \param p  Port to set as a slave
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 * 
 *  \note It is highly recommended to use the \c port_protocol_* functions to put a
 *        port into its desired mode as the order of operations is critical.
 *  \see xcore/port_protocol.h
 */
inline void port_set_slave(port_id_t p)
{
  _port_set_slave(p);
}

/** \brief Set the port to use no ready signals (default state).
 *
 *  Changes a port to not use ready signals. A port can be configured
 *  to use strobes or handshaking signals using port_set_ready_strobed() or
 *  port_set_ready_handshake().
 *
 *  \attention the port must be a \c master port otherwise this function will raise
 *             an exception.
 *
 *  \param p  Port to change to not use ready signals
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 * 
 *  \note It is highly recommended to use the \c port_protocol_* functions to put a
 *        port into its desired mode as the order of operations is critical.
 *  \see xcore/port_protocol.h
 */
inline void port_set_no_ready(port_id_t p)
{
  _port_set_no_ready(p);
}

/** \brief Set the port to use a single strobe.
 *
 *  Changes a port to not use ready signals. A port can be configured
 *  to use strobes or handshaking signals using port_set_ready_strobed() or
 *  port_set_ready_handshake().
 *
 *  \attention the port must be a buffered port otherwise this function will raise
 *             an exception.
 *
 *  \param p  Port to change to not use ready signals
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *
 *  \note It is highly recommended to use the \c port_protocol_* functions to put a
 *        port into its desired mode as the order of operations is critical.
 *  \see xcore/port_protocol.h
 */
inline void port_set_ready_strobed(port_id_t p)
{
  _port_set_ready_strobed(p);
}

/** \brief Set the port to be fully handshaken.
 *
 *  Changes a port to use both a ready input and drive a ready
 *  output in order to control when data is sampled or written.
 *
 *  \attention the port must be a master buffered port otherwise this function will
 *             raise an exception.
 *
 *  \param p  Port to change to not use ready signals
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *
 *  \note It is highly recommended to use the \c port_protocol_* functions to put a
 *        port into its desired mode as the order of operations is critical.
 *  \see xcore/port_protocol.h
 */
inline void port_set_ready_handshake(port_id_t p)
{
  _port_set_ready_handshake(p);
}

/** \brief Gets the timestamp of the last input or output operation on a port.
 *
 *  \param p  The port to get the timestamp from  
 *  \return   The timestamp of the last operation
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline int16_t port_get_trigger_time(port_id_t p)
{
  return _port_get_trigger_time(p);
}

/** \brief Set the timestamp at which the port will input/output data.
 *
 *  Sets the time condition for the next input or output on
 *  a port.
 *  If the port is unbuffered or the buffer is empty/full a call to port_in()
 *  or port_out() will pause until the specified time.
 *  The trigger is cleared by a input/output or by calling port_clear_trigger_time().
 *
 *  \param p  The port to set the condition on
 *  \param t  The port timestamp to match
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline void port_set_trigger_time(port_id_t p, int16_t t)
{
  _port_set_trigger_time(p, t);
}

/** \brief Clear the timestamp trigger on a port.
 *
 *  Clears any trigger_time condition on the port so the next input or
 *  output will happen unconditionally in respect to the timestamp.
 *  This function does not clear the trigger_in condition on the port.
 *
 *  \param p  The port to clear the trigger_time on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_clear_trigger_time(port_id_t p)
{
  _port_clear_trigger_time(p);
}

/** \brief Setup an event to trigger on a port when its input value matches.
 *
 *  On a unbuffered port the trigger will apply to all future inputs
 *  until the trigger is set again.
 *  On an buffered port the trigger will only hold for the next input
 *  after which the trigger_in_equal will be cleared.
 *
 *  \param p  The port to set the trigger on
 *  \param v  The value to match
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_trigger_in_equal(port_id_t p, uint32_t v)
{
  _port_set_trigger_in_equal(p, v);
}

/** \brief Setup an event to trigger on a port when its input value does not matches.
 *
 *  On a unbuffered port the trigger will apply to all future inputs
 *  until the trigger is set again.
 *  On an buffered port the trigger will only hold for the next input
 *  after which the trigger_in_not_equal will be cleared.
 *
 *  \param p    The port to set the trigger on
 *  \param v    The value to not match
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_set_trigger_in_not_equal(port_id_t p, uint32_t v)
{
  _port_set_trigger_in_not_equal(p, v);
}

/** \brief Clear the in trigger on a port.
 *
 *  Clears any trigger_in condition on the port so the next input
 *  will happen unconditionally in respect to the input value.
 *  This function does not clear the trigger_time condition on the port.
 *
 *  \param p    The port to clear the trigger_in on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_clear_trigger_in(port_id_t p)
{
  _port_clear_trigger_in(p);
}

/** \brief Peek at the value on a port.
 *
 *  Peeking a port returns the current value on the pins of a port, regardless
 *  of whether the port is an output or input and without affecting its direciton.
 *  Peek will not pause, regardless of any triggers that have been set.
 *
 *  \param p     Port to be peeked
 *  \return      The current value on the pins
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 */
inline uint32_t port_peek(port_id_t p)
{
  return _port_peek(p);
}

/** \brief Outputs a value onto a port.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins on
 *  the next clock cycle. In the case of a buffered port, the data will be
 *  stored in the buffer, and be serialised onto the output pins.
 *
 *  If there is a time trigger setup and the port is unbuffered or the buffer is full
 *  the call will pause until the specified time.
 *
 *  \param p     Port to output to
 *  \param data  Value to output
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_out(port_id_t p, uint32_t data)
{
  _port_out(p, data);
}

/** \brief Input a value from a port.
 *
 *  For unbuffered port with no trigger, the data will be whatever is on the input pins.
 *  For unbuffered port with a trigger, the data will be the value read when the
 *  trigger fired. The call will pause if the trigger has not yet fired.
 *  For buffered port, this function will pause until the buffer is filled up with
 *  deserialised data.
 *
 *  \param p  Port to input from
 *  \return   The inputted data
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in(port_id_t p)
{
  return _port_in(p);
}

/** \brief Outputs a value onto a port and shift the output data.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins on
 *  the next clock cycle. In the case of a buffered port, the data will be stored
 *  in the buffer, and be serialised onto the output pins.
 *
 *  If there is a time trigger setup and the port is unbuffered or the buffer is full
 *  the call will pause until the specified time.
 *
 *  \param p     Port to output to
 *  \param data  \a data is shifted right by the transfer width of the port,
 *               with the bits shifting out onto the port.
 *  \return      The remaining shifted bits of \a data
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_out_shift_right(port_id_t p, uint32_t data)
{
  return _port_out_shift_right(p, data);
}

/** \brief Input a value from a port and shift the data.
 *
 *  For unbuffered port with no trigger, the data will be whatever is on the input pins.
 *  For unbuffered port with a trigger, the data will be the value read when the
 *  trigger fired. The call will pause if the trigger has not yet fired.
 *  For buffered port, this function will pause until the buffer is filled up with
 *  deserialised data.
 *
 *  \param p     Port to input from
 *  \param data  Initial value to shift input data into
 *  \return      Result of shifting \a data right by the port width and storing the
 *               input data in the most significant bits
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in_shift_right(port_id_t p, uint32_t data)
{
  return _port_in_shift_right(p, data);
}

/** \brief Outputs a value onto a port at a specified port timestamp.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins when
 *  on the clock cycle that moves the port timestamp to the specified time. In
 *  the case of a buffered port, the data will be stored in the buffer, and
 *  be serialised onto the output pins at the point that the time is reached.
 *
 *  \param p     Port to output to
 *  \param t     The timestamp to do the output on
 *  \param data  Value to output
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_out_at_time(port_id_t p, int16_t t, uint32_t data)
{
  _port_set_trigger_time(p, t);
  _port_out(p, data);
}

/** \brief Input data from a port when its counter is at a specific time.
 *
 *  In the case of an unbuffered port, the data will be inputted when the
 *  counter reaches time \a t. In the case of a buffered port, an input
 *  will wait until the given time and then will start capturing data,
 *  returning a value when the buffer is full.
 *
 *  \param p  Port to input from
 *  \param t  The timestamp to do input on
 *  \return   The inputted data
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in_at_time(port_id_t p, int16_t t)
{
  _port_set_trigger_time(p, t);
  return _port_in(p);
}

/** \brief Outputs a value onto a port at a specified time and shifts the output data.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins when
 *  on the clock cycle that moves the port counter to the specified time. In
 *  the case of a buffered port, the data will be stored in the buffer, and
 *  be serialised onto the output pins at the point that the time is reached.
 *
 *  \param p     Port to output to
 *  \param t     The timestamp of the output
 *  \param data  data is shifted right by the transfer width of the port,
 *               with the bits shifting out onto the port.
 *  \return      The remaining shifted bits
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_out_shift_right_at_time(port_id_t p, int16_t t, uint32_t data)
{
  _port_set_trigger_time(p, t);
  return _port_out_shift_right(p, data);
}

/** \brief Input data from a port at a specific time and shift the data.
 *
 *  In the case of an unbuffered port, the data will be inputted when the
 *  counter reaches time \a t. In the case of a buffered port, an input
 *  will wait until the given time and then will start capturing data,
 *  returning a value when the buffer is full.
 *
 *  \param p     Port to input from
 *  \param t     The timestamp to do input on
 *  \param data  Initial value to shift input data into
 *  \return      Result of shifting \a data right by the port width and storing the
 *               input data in the most significant bits
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in_shift_right_at_time(port_id_t p, int16_t t, uint32_t data)
{
  _port_set_trigger_time(p, t);
  return _port_in_shift_right(p, data);
}


/** \brief Input data from a port when its pins match a specific value.
 *
 *  In the case of an unbuffered port, the data inputted
 *  be identical to the value. In the case of a buffered port, an input
 *  will wait until the value appears on the pins and then return that
 *  value and some previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \return       The inputted data
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in_when_pinseq(port_id_t p, port_type_t pt, uint32_t value)
{
  _port_set_trigger_in_equal(p , value);
  uint32_t data = _port_in(p);
  if (pt==PORT_UNBUFFERED)
  {
    _port_clear_trigger_in(p);
  }
  return data;
}

/** \brief Input data from a port when its pins do not match a specific value.
 *
 *  In the case of an unbuffered port, the inputted data
 *  will be the non-matching pin values. In the case of a buffered port,
 *  this function will wait until a non matching value appears on the pins, and
 *  then return that value and previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \return       The inputted data
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in_when_pinsneq(port_id_t p, port_type_t pt, uint32_t value)
{
  _port_set_trigger_in_not_equal(p , value);
  uint32_t data = _port_in(p);
  if (pt==PORT_UNBUFFERED)
  {
    _port_clear_trigger_in(p);
  }
  return data;
}


/** \brief Input data from a port when its pins match a specific value and shift the data.
 *
 *  In the case of an unbuffered port, the data inputted
 *  be identical to the value. In the case of a buffered port, an input
 *  will wait until the value appears on the pins and then return that
 *  value and some previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \param data   Initial value to shift input data into
 *  \return       Result of shifting \a data right by the port width and storing the
 *                input data in the most significant bits
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in_shift_right_when_pinseq(port_id_t p, port_type_t pt, uint32_t value, uint32_t data)
{
  _port_set_trigger_in_equal(p , value);
  uint32_t new_data = _port_in_shift_right(p, data);
  if (pt==PORT_UNBUFFERED)
  {
    _port_clear_trigger_in(p);
  }
  return new_data;
}

/** \brief Input data from a port when its pins do not match a specific value  and shift the data.
 *
 *  In the case of an unbuffered port, the inputted data
 *  will be the non-matching pin values. In the case of a buffered port,
 *  this macro will wait until a non matching value appears on the pins, and
 *  then return that value and previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param pt     If port is buffered or unbuffered.
 *  \param value  The value to match against the pins
 *  \param data   Initial value to shift input data into
 *  \return       Result of shifting \a data right by the port width and storing the
 *                input data in the most significant bits
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline uint32_t port_in_shift_right_when_pinsneq(port_id_t p, port_type_t pt, uint32_t value, uint32_t data)
{
  _port_set_trigger_in_not_equal(p, value);
  uint32_t new_data = _port_in_shift_right(p, data);
  if (pt==PORT_UNBUFFERED)
  {
    _port_clear_trigger_in(p);
  }
  return new_data;
}

/** \brief Clears the buffer used by a port.
 *
 *  Any data sampled by the port which has not been input by the processor is
 *  discarded. Any data output by the processor which has not been driven by the
 *  port is discarded. If the port is in the process of serialising output, it
 *  is interrupted immediately. If a pending output would have caused a change
 *  in direction of the port then that change of direction does not take place.
 *  If the port is driving a value on its pins when this function is called then
 *  it continues to drive the value until an output statement changes the value
 *  driven.
 *
 *  \param p  The port whose buffer is to be cleared
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline void port_clear_buffer(port_id_t p)
{
  _port_clear_buffer(p);
}

/** \brief Ends the current input on a buffered port.
 *
 *  The number of bits sampled by the port but not yet input by the processor is
 *  returned. This count includes both data in the transfer register and data in
 *  the shift register used for deserialisation.
 *  Subsequent inputs on the port return transfer-width bits of data
 *  until there is less than one transfer-width bits of data remaining.
 *  Any remaining data can be read with one further input, which
 *  returns transfer-width bits of data with the remaining buffered data
 *  in the most significant bits of this value.
 *
 *  \param p  The port to end the current input on
 *  \return   The number of bits of data remaining
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline size_t port_endin(port_id_t p)
{
  return _port_endin(p);
}

/** \brief Force an input on a buffered port.
 *
 *  Perform an input on a buffered port even if the buffer is only partially full.
 *
 *  \param p          The port to do the input on
 *  \param[out] data  The inputted data

 *  \return The number of bits input
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid \a data argument.
 */
inline size_t port_force_input(port_id_t p, uint32_t *data)
{
  size_t num = _port_endin(p);
  *data = _port_in(p);
  return num;
}

/** \brief Setup interrupt event on a port.
 *
 *  Once the event is setup you need to call port_enable_trigger() to enable it.
 *
 *  \param p         The port to setup the interrupt event on
 *  \param[in] data  The value to be passed to the interrupt_callback_t function
 *                   On XS1 bit 16 must be set (see ENUM_ID_BASE)
 *  \param func      The interrupt_callback_t function to handle events
 *
 *  \see ENUM_ID_BASE
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in data.
 */
inline void port_setup_interrupt_callback(port_id_t p, void *data, interrupt_callback_t func)
{
  _resource_setup_interrupt_callback(p, data, func);
  _port_clear_trigger_in(p);
}

/** \brief Enable select & interrupt events on a port.
 *
 *  Prior to enabling, port_setup_select(), port_setup_select_callback() or
 *  port_setup_interrupt_callback() must have been called.
 *  Events can be temporarily disabled and re-enabled using port_disable_trigger()
 *  and port_enable_trigger().
 *  When the event fires, the value must be read from the port to clear the event.
 *
 *  \param p  The port to enable events on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline void port_enable_trigger(port_id_t p)
{
  _resource_enable_trigger(p);
}

/** \brief Disable select & interrupt events for a given port.
 *
 *  This prevents any further events being triggered by a given port.
 *
 *  \param p  The port to disable events on
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline void port_disable_trigger(port_id_t p)
{
  _resource_disable_trigger(p);
}

/** \brief Sets the trigger value for a port with a configured trigger.
 *
 *  Changes only the trigger value of a port which has already been configured
 *  to trigger on a value using port_set_trigger_in_equal() or port_set_trigger_in_not_equal().
 *
 *  \param p      The port to change the trigger value of
 *  \param value  The new trigger value to set
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline void port_set_trigger_value(port_id_t p, uint32_t value)
{
  _port_set_trigger_value(p, value);
}

#endif // !defined(__XC__)

#endif // __xcore_c_port_h__
