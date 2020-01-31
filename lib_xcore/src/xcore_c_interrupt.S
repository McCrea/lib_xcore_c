// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved

#ifdef __XS2A__
# define _XCORE_STACK_ALIGN(n)   ((n+1)/2)*2
#else
# define _XCORE_STACK_ALIGN(n)   n
#endif

.globl __xcore_interrupt_permitted_common
.align 2  // We arrive in single issue mode.
.type  __xcore_interrupt_permitted_common,@function
.cc_top __xcore_interrupt_permitted_common.function,__xcore_interrupt_permitted_common
__xcore_interrupt_permitted_common:
  // This is the body of the __xcore_interrupt_permitted_XXX functions.
  // We have an active _XCORE_STACK_ALIGN(3) word sp frame.
  // sp[2] = saved r5
  // sp[1] = saved r4
  // sp[0] will be trashed when we set up the kstack.
  // r0-3 = the original arguments not to be touched.
  // r4 = kstack words needed.
  // r5 = root_function.
  // Set up the kstack on our stack.
  //
  // Stack before:
  //
  // N-X:  --------- (end of stack)
  // ........... (enough space for stack required by root_function)
  // N-R4: ----------
  // ........... (enough space for stack required by ISRs)
  // N-2:  ----------
  // N-1:  ----------
  // N-0:  ---------- <- SP
  //
  ldaw r11, sp[0]
  ldaw r11, r11[-r4]  // Calculate the new SP address (_kstack_words into the current stack)
  stw r11, sp[0]      // Store the new SP address onto the top of the current stack
  ldaw r4, sp[0]      // Keep a record of the current SP
  krestsp 0           // Makes the current SP the KSP and pops the new SP address into SP
  //
  // Stack after:
  //
  // N-X:  --------- (end of stack)
  // ........... (enough space for stack required by root_function)
  // N-R4: ---------- <- SP
  // ........... (enough space for stack required by ISRs)
  // N-2:  ----------
  // N-1:  ----------
  // N-0:  ---------- <- KSP
  //
  // Call the root_function.
  bla r5
  set sp, r4          // Restore SP
  // And return.
  ldw r4, sp[1]
  ldw r5, sp[2]
  retsp _XCORE_STACK_ALIGN(3)
.cc_bottom __xcore_interrupt_permitted_common.function
.set   __xcore_interrupt_permitted_common.nstackwords, 0
.globl __xcore_interrupt_permitted_common.nstackwords
.set   __xcore_interrupt_permitted_common.maxcores, 1
.globl __xcore_interrupt_permitted_common.maxcores
.set   __xcore_interrupt_permitted_common.maxtimers, 0
.globl __xcore_interrupt_permitted_common.maxtimers
.set   __xcore_interrupt_permitted_common.maxchanends, 0
.globl __xcore_interrupt_permitted_common.maxchanends
.size  __xcore_interrupt_permitted_common, . - __xcore_interrupt_permitted_common


.globl __xcore_interrupt_callback_common
.align 2  // We arrive in single issue mode.
.type  __xcore_interrupt_callback_common,@function
.cc_top __xcore_interrupt_callback_common.function,__xcore_interrupt_callback_common
__xcore_interrupt_callback_common:
  // This is the body of the __xcore_interrupt_callback_XXX functions.
  // We have an active _XCORE_STACK_ALIGN(7) word ksp frame.
  // r1 = interrupt_callback_t function
  stw lr, sp[6]
  // ksp[5] = r11
  stw r0, sp[4]
  // ksp[3] = r1
  stw r2, sp[2]
  stw r3, sp[1]
  // ksp[0] will be trashed by the bla.
  // And call the interrupt_callback_t.
  get r11, ed
  add r0, r11, 0
  bla r1
  // Restore the state and return.
  ldw r3, sp[1]
  ldw r2, sp[2]
  ldw r1, sp[3]
  ldw r0, sp[4]
  ldw r11, sp[5]
  ldw lr, sp[6]
  krestsp _XCORE_STACK_ALIGN(7)
  kret
.cc_bottom __xcore_interrupt_callback_common.function
.set   __xcore_interrupt_callback_common.nstackwords, 0
.globl __xcore_interrupt_callback_common.nstackwords
.set   __xcore_interrupt_callback_common.maxcores, 1
.globl __xcore_interrupt_callback_common.maxcores
.set   __xcore_interrupt_callback_common.maxtimers, 0
.globl __xcore_interrupt_callback_common.maxtimers
.set   __xcore_interrupt_callback_common.maxchanends, 0
.globl __xcore_interrupt_callback_common.maxchanends
.size  __xcore_interrupt_callback_common, . - __xcore_interrupt_callback_common
