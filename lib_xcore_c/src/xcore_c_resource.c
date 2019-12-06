// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore/_support/xcore_c_resource_impl.h"

void _resource_setup_callback(resource_t r, void *data, callback_function func, uint32_t type)
{
#if !defined(__XS2A__)
  xassert( ((uint32_t)data >> 16) == 0x1 && msg("On XS1 bit 16 will always be set in the data returned from an event"));
#endif
  asm volatile("add r11, %0, 0" : : "r" (func) : /* clobbers */ "r11");
  asm volatile("setv res[%0], r11" : : "r" (r));
  asm volatile("add r11, %0, 0" : : "r" (data) : /* clobbers */ "r11");
  asm volatile("setev res[%0], r11" : : "r" (r));
  asm volatile("setc res[%0], %1" : : "r" (r), "r" (type)); // Raise interrupts or events
}
