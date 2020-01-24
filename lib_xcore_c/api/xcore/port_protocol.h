// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 *  \brief Helper functinos for port usage patterns
 */

#include <stdint.h>
#include <xcore/_support/xcore_c_common.h>
#include <xcore/_support/xcore_c_port_impl.h>
#include <xcore/_support/xcore_c_clock_impl.h>
#include <xcore/clock.h>
#include <xcore/port.h>

/** \brief Configure a port to be a clocked input port in handshake mode.
 *
 *  If the ready-in or ready-out ports are not 1-bit ports, an exception is raised.
 *  The ready-out port is asserted on the falling edge of the clock when the
 *  port's buffer is not full. The port samples its pins on its sampling edge
 *  when both the ready-in and ready-out ports are asserted.
 *
 *  By default the port's sampling edge is the rising edge of clock. This can be
 *  changed by the function port_set_sample_falling_edge().
 *
 *  \note A handshaken port must be buffered, so this function will also make
 *        the port buffered.
 *
 *  \param p         The port to configure
 *  \param readyin   A 1-bit port to use for the ready-in signal
 *  \param readyout  A 1-bit port to use for the ready-out signal
 *  \param clk       The clock used to configure the port
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin/readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
_XCORE_C_EXFUN
inline void port_protocol_in_handshake(port_t p, port_t readyin, port_t readyout, xclock_t clk)
{
  _port_set_inout_data(p);
  _port_set_buffered(p);
  /* ensure port is in input mode. */
  (void) _port_in(p);
  _clock_set_ready_src(clk, readyin);
  _port_set_clock(p, clk);
  _port_set_ready_handshake(p);
  _port_clear_buffer(p);
  _port_set_out_ready(readyout, p);
}

/** \brief Configures a port to be a clocked output port in handshake mode.
 *
 *  If the ready-in or ready-out ports are not 1-bit ports an exception is
 *  raised. the port drives the initial value on its pins until an
 *  output statement changes the value driven.
 *
 *  The ready-in port is read on the sampling edge of the port. outputs are driven
 *  on the next falling edge of the clock where the previous value read from the
 *  ready-in port was high.
 *
 *  On the falling edge of the clock the ready-out port is driven high
 *  if data is output on that edge, otherwise it is driven low.
 *
 *  By default the port's sampling edge is the rising edge of clock. this can be
 *  changed by the function port_set_sample_falling_edge().
 *
 *  \note A handshaken port must be buffered, so this function will also make
 *        the port buffered.
 *
 *  \param p         the port to configure
 *  \param readyin   a 1-bit port to use for the ready-in signal
 *  \param readyout  a 1-bit port to use for the ready-out signal
 *  \param clk       the clock used to configure the port
 *  \param initial   the initial value to output on the port
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin/readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
_XCORE_C_EXFUN
inline void port_protocol_out_handshake(port_t p, port_t readyin, port_t readyout, xclock_t clk, uint32_t initial)
{
  _port_set_inout_data(p);
  _port_set_buffered(p);
  /* ensure port is in output mode. */
  _port_out(p, initial);
  _clock_set_ready_src(clk, readyin);
  _port_set_clock(p, clk);
  _port_set_ready_handshake(p);
  _port_clear_buffer(p);
  _port_set_out_ready(readyout, p);
}

/** \brief Configures a port to be a clocked input port in strobed master mode.
 *
 *  If the ready-out port is not a 1-bit port, an exception is raised.
 *  the ready-out port is asserted on the falling edge of the clock when the
 *  port's buffer is not full. the port samples its pins on its sampling edge
 *  after the ready-out port is asserted.
 *
 *  By default the port's sampling edge is the rising edge of clock. this can be
 *  changed by the function set_port_sample_delay().
 *
 *  \note A strobed port must be buffered, so this function will also make the
 *        port buffered.
 *
 *  \param p         the port to configure
 *  \param readyout  a 1-bit port to use for the ready-out signal
 *  \param clk       the clock used to configure the port
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
_XCORE_C_EXFUN
inline void port_protocol_in_strobed_master(port_t p, port_t readyout, xclock_t clk)
{
  _port_set_inout_data(p);
  _port_set_buffered(p);
  /* ensure port is in input mode. */
  (void) _port_in(p);
  _port_set_clock(p, clk);
  _port_set_ready_strobed(p);
  _port_set_master(p);
  _port_clear_buffer(p);
  _port_set_out_ready(readyout, p);
}

/** \brief Configures a port to be a clocked output port in strobed master mode.
 *
 *  If the ready-out port is not a 1-bit port, an exception is raised.
 *  the port drives the initial value on its pins until an
 *  output statement changes the value driven. outputs are driven on the next
 *  falling edge of the clock. on the falling edge of the clock the ready-out
 *  port is driven high if data is output on that edge, otherwise it is driven
 *  low.
 *
 *  \note A strobed port must be buffered, so this function will also make the
 *        port buffered.
 *
 *  \param p         the port to configure
 *  \param readyout  a 1-bit port to use for the ready-out signal
 *  \param clk       the clock used to configure the port
 *  \param initial   the initial value to output on the port
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyout not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
_XCORE_C_EXFUN
inline void port_protocol_out_strobed_master(port_t p, port_t readyout, xclock_t clk, uint32_t initial)
{
  _port_set_inout_data(p);
  _port_set_buffered(p);
  /* ensure port is in output mode. */
  _port_out(p, initial);
  _port_set_clock(p, clk);
  _port_set_ready_strobed(p);
  _port_set_master(p);
  _port_clear_buffer(p);
  _port_set_out_ready(readyout, p);
}

/** \brief Configures a port to be a clocked input port in strobed slave mode.
 *
 *  If the ready-in port is not a 1-bit port, an exception is raised.
 *  the port samples its pins on its sampling edge when the ready-in signal is
 *  high. by default the port's sampling edge is the rising edge of clock. this
 *  can be changed by the function set_port_sample_delay().
 *
 *  \note A strobed port must be buffered, so this function will also make the
 *        port buffered.
 *
 *  \param p        the port to configure
 *  \param readyin  a 1-bit port to use for the ready-in signal
 *  \param clk      the clock used to configure the port
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
_XCORE_C_EXFUN
inline void port_protocol_in_strobed_slave(port_t p, port_t readyin, xclock_t clk)
{
  _port_set_inout_data(p);
  _port_set_buffered(p);
  /* ensure port is in input mode. */
  (void) _port_in(p);
  _clock_set_ready_src(clk, readyin);
  _port_set_clock(p, clk);
  _port_set_ready_strobed(p);
  _port_set_slave(p);
  _port_clear_buffer(p);
}

/** \brief Configures a port to be a clocked output port in strobed slave mode.
 *
 *  If the ready-in port is not a 1-bit port, an exception is raised.
 *  the port drives the initial value on its pins until an
 *  output statement changes the value driven. the ready-in port is read on the
 *  port's sampling edge. outputs are driven on the next falling edge
 *  of the clock where the previous value read from the ready-in port is high.
 *  by default the port's sampling edge is the rising edge of clock. this
 *  can be changed by the function set_port_sample_delay().
 *
 *  \note A strobed port must be buffered, so this function will also make the
 *        port buffered.
 *
 *  \param p        the port to configure
 *  \param readyin  a 1-bit port to use for the ready-in signal
 *  \param clk      the clock used to configure the port
 *  \param initial  the initial value to output on the port
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port/clock
 *                                    or clock is running,
 *                                    or readyin not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing a port/clock
 */
_XCORE_C_EXFUN
inline void port_protocol_out_strobed_slave(port_t p, port_t readyin, xclock_t clk, uint32_t initial)
{
  _port_set_inout_data(p);
  _port_set_buffered(p);
  /* ensure port is in output mode. */
  _port_out(p, initial);
  _clock_set_ready_src(clk, readyin);
  _port_set_clock(p, clk);
  _port_set_ready_strobed(p);
  _port_set_slave(p);
}
