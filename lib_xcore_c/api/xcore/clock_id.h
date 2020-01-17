#pragma once

#include <platform.h>

/** \file
 *  \brief Provides an enumeration of the available clock resources.
 *  \see \ref clock.h - \copybrief clock.h
 */

/** \typedef clock_id_t
 *  \brief A clock block identifier
 *  \note Clock resources are not allocated from a pool - one of the below clocks must be chosen
 *        and enabled using clock_enable() before use.
 */
typedef enum {
  clock_ref = XS1_CLKBLK_REF,
  clock_1 = XS1_CLKBLK_1,
  clock_2 = XS1_CLKBLK_2,
  clock_3 = XS1_CLKBLK_3,
  clock_4 = XS1_CLKBLK_4,
  clock_5 = XS1_CLKBLK_5,
} clock_id_t;

