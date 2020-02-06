// Copyright (c) 2020, XMOS Ltd, All rights reserved

#include <xcore/_support/xcore_macros.h>

.text

.globl __xcore_unsynchronised_thread_end
#ifdef __XS2A__
.issue_mode single
#endif
.align _XCORE_CODE_ALIGNMENT
.type  __xcore_unsynchronised_thread_end,@function
.cc_top __xcore_unsynchronised_thread_end.function,__xcore_unsynchronised_thread_end
__xcore_unsynchronised_thread_end:
  freet

.cc_bottom __xcore_unsynchronised_thread_end.function
.set   __xcore_unsynchronised_thread_end.nstackwords, 0
.globl __xcore_unsynchronised_thread_end.nstackwords
.set   __xcore_unsynchronised_thread_end.maxcores, 1
.globl __xcore_unsynchronised_thread_end.maxcores
.set   __xcore_unsynchronised_thread_end.maxtimers, 0
.globl __xcore_unsynchronised_thread_end.maxtimers
.set   __xcore_unsynchronised_thread_end.maxchanends, 0
.globl __xcore_unsynchronised_thread_end.maxchanends
.size  __xcore_unsynchronised_thread_end, . - __xcore_unsynchronised_thread_end



.globl __xcore_synchronised_thread_end
#ifdef __XS2A__
.issue_mode single
#endif
.align _XCORE_CODE_ALIGNMENT
.type  __xcore_synchronised_thread_end,@function
.cc_top __xcore_synchronised_thread_end.function,__xcore_synchronised_thread_end
__xcore_synchronised_thread_end:
  ssync

.cc_bottom __xcore_synchronised_thread_end.function
.set   __xcore_synchronised_thread_end.nstackwords, 0
.globl __xcore_synchronised_thread_end.nstackwords
.set   __xcore_synchronised_thread_end.maxcores, 1
.globl __xcore_synchronised_thread_end.maxcores
.set   __xcore_synchronised_thread_end.maxtimers, 0
.globl __xcore_synchronised_thread_end.maxtimers
.set   __xcore_synchronised_thread_end.maxchanends, 0
.globl __xcore_synchronised_thread_end.maxchanends
.size  __xcore_synchronised_thread_end, . - __xcore_synchronised_thread_end
