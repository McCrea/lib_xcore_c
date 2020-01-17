#pragma once

#include <platform.h>

/** \file
 *  \brief Provides an enumeration of the available ports.
 *  \see \ref port.h - \copybrief port.h
 */


/** \brief A port identifier
 *
 *  Port resources are not allocated from a pool but must be selected manually from
 *  the following and enabled with port_enable().
 */
typedef enum {
  port_1A  = XS1_PORT_1A,
  port_1B  = XS1_PORT_1B,
  port_1C  = XS1_PORT_1C,
  port_1D  = XS1_PORT_1D,
  port_1E  = XS1_PORT_1E,
  port_1F  = XS1_PORT_1F,
  port_1G  = XS1_PORT_1G,
  port_1H  = XS1_PORT_1H,
  port_1I  = XS1_PORT_1I,
  port_1J  = XS1_PORT_1J,
  port_1K  = XS1_PORT_1K,
  port_1L  = XS1_PORT_1L,
  port_1M  = XS1_PORT_1M,
  port_1N  = XS1_PORT_1N,
  port_1O  = XS1_PORT_1O,
  port_1P  = XS1_PORT_1P,
  port_4A  = XS1_PORT_4A,
  port_4B  = XS1_PORT_4B,
  port_4C  = XS1_PORT_4C,
  port_4D  = XS1_PORT_4D,
  port_4E  = XS1_PORT_4E,
  port_4F  = XS1_PORT_4F,
  port_8A  = XS1_PORT_8A,
  port_8B  = XS1_PORT_8B,
  port_8C  = XS1_PORT_8C,
  port_8D  = XS1_PORT_8D,
  port_16A = XS1_PORT_16A,
  port_16B = XS1_PORT_16B,
  port_16C = XS1_PORT_16C,
  port_16D = XS1_PORT_16D,
  port_32A = XS1_PORT_32A,
  port_32B = XS1_PORT_32B,
} port_id_t;