lib_sourcedir := ./src
lib_c_sources := xcore_c_resource.c
#	xcore_c_chan.c \
#	xcore_c_clock.c \
#	xcore_c_interrupt.c \
#	xcore_c_lock.c \
#	xcore_c_select.c \
#	xcore_c_hwtimer.c \
#	xcore_c_port.c \
#	xcore_c_resource.c

lib_asm_sources := \
	xcore_c_interrupt.S \
	xcore_c_select.S \
	xcore_c_thread.S

lib_xc_sources := 
lib_name := xcore_c
lib_incdirs := \
	./api \
	../../lib_xassert/lib_xassert/api

include xlib.mk

