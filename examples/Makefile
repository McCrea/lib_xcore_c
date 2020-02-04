.SUFFIXES:

.PHONY: _default
_default: all

COMPILER ?= xcc
LINKER ?= xcc
build_dir := ./.build
TARGET ?= XR-USB-AUDIO
lib_xcore_c := ../lib_xcore/libxcore.a

include $(wildcard $(build_dir)/*.d)

$(build_dir):
	mkdir $@

.INTERMEDIATE: $(build_dir)/%.o
$(build_dir)/%.o : %.c | $(build_dir)
	$(COMPILER) -c $< -o $@ -O3 -MD -MF $|/$*.d $(CFLAGS) -D_clock_defined  -isystem/usr/share/xmos/target/include/clang -isystem/usr/share/xmos/target/include -isystem ../lib_xcore/api --target=xcore #-target=$(TARGET)

%.xe : $(build_dir)/%.o $(lib_xcore_c)
	$(LINKER) $^ -o $@ -target=$(TARGET) 


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
	assert \
	custom_select \
	interrupt \
	chanend_ct \
	\
	internal_meta_macro \

.PHONY: $(examples)
$(examples) : % : %.xe

.PHONY: all
all: $(examples)

.PHONY: clean
clean:
	rm -rf $(wildcard $(build_dir))
	rm -f $(wildcard $(examples:%=%.xe))

