.SUFFIXES:

.PHONY: _default
_default: all

XCC ?= xcc
build_dir := .build
TARGET ?= XR-USB-AUDIO
lib_xcore_c := ../libxcore_c.a

include $(wildcard $(build_dir)/*.d)

$(build_dir):
	mkdir $@

.INTERMEDIATE: $(build_dir)/%.o
$(build_dir)/%.o : %.c | $(build_dir)
	$(XCC) -c $< -o $@ -O3 -MD -MF $|/$*.d -isystem ../api -target=$(TARGET) -I../../../lib_xassert/lib_xassert/api

%.xe : $(build_dir)/%.o $(lib_xcore_c)
	$(XCC) $^ -o $@ -target=$(TARGET) 


examples := \
	hello_world_pfunc \
	hello_world_pjob \
	hello_world_threads \
	hello_world_thread_group \
	lock \
	channel \
	timer_wait \
	port_invert_peek \
	port_condition \
	port_ready_signal \



.PHONY: $(examples)
$(examples) : % : %.xe

.PHONY: all
all: $(examples)

.PHONY: clean
clean:
	rm -rf $(wildcard $(build_dir))
	rm -f $(wildcard $(examples:%=%.xe))

