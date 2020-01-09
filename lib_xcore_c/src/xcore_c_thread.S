
.text

.globl _xcore_c_unsynchronised_thread_end
#ifdef __XS2A__
.issue_mode single
#endif
.align 2
.type  _xcore_c_unsynchronised_thread_end,@function
.cc_top _xcore_c_unsynchronised_thread_end.function,_xcore_c_unsynchronised_thread_end
_xcore_c_unsynchronised_thread_end:
  freet

.cc_bottom _xcore_c_unsynchronised_thread_end.function
.set   _xcore_c_unsynchronised_thread_end.nstackwords, 0
.globl _xcore_c_unsynchronised_thread_end.nstackwords
.set   _xcore_c_unsynchronised_thread_end.maxcores, 1
.globl _xcore_c_unsynchronised_thread_end.maxcores
.set   _xcore_c_unsynchronised_thread_end.maxtimers, 0
.globl _xcore_c_unsynchronised_thread_end.maxtimers
.set   _xcore_c_unsynchronised_thread_end.maxchanends, 0
.globl _xcore_c_unsynchronised_thread_end.maxchanends
.size  _xcore_c_unsynchronised_thread_end, . - _xcore_c_unsynchronised_thread_end



.globl _xcore_c_synchronised_thread_end
#ifdef __XS2A__
.issue_mode single
#endif
.align 2
.type  _xcore_c_synchronised_thread_end,@function
.cc_top _xcore_c_synchronised_thread_end.function,_xcore_c_synchronised_thread_end
_xcore_c_synchronised_thread_end:
  ssync

.cc_bottom _xcore_c_synchronised_thread_end.function
.set   _xcore_c_synchronised_thread_end.nstackwords, 0
.globl _xcore_c_synchronised_thread_end.nstackwords
.set   _xcore_c_synchronised_thread_end.maxcores, 1
.globl _xcore_c_synchronised_thread_end.maxcores
.set   _xcore_c_synchronised_thread_end.maxtimers, 0
.globl _xcore_c_synchronised_thread_end.maxtimers
.set   _xcore_c_synchronised_thread_end.maxchanends, 0
.globl _xcore_c_synchronised_thread_end.maxchanends
.size  _xcore_c_synchronised_thread_end, . - _xcore_c_synchronised_thread_end
