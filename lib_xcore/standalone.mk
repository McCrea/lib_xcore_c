.SUFFIXES:

export XCORE_TARGET ?= XR-USB-AUDIO

export GLOBAL_COMPILER_FLAGS ?=
global_compiler_flags := $(GLOBAL_COMPILER_FLAGS)

export XCC_C_INCLUDE_PATH ?= 
system_include_dirs ?= ./api

#SECONDARY: %.o
#.o : %.c
#$(CLANG) -c -o $@ --target=xcore $(dependency_file_flag) -MF $*.d  $(addprefix -isystem,$(system_include_dirs)) $(addprefix -I,$(lib_incdirs)) $(global_compiler_flags) $<
#
#SECONDARY: $(xc_objects)
#(xc_objects) : %.xo : %.xc 
#$(XCC) -c $< -o $@ -target=$(XCORE_TARGET) $(addprefix -I,$(lib_incdirs)) $(global_compiler_flags) $(dependency_file_flag) -MF $*.xd


# --------------------------------------------------------------



C_COMPILER := xcc
ASSEMBLER ?= xcc
AR ?= ar

build_dir := ./build
VPATH = ./src
lib_incdirs := \
        ./api \

CONFIG ?=
config_dir = $(build_dir)/$(CONFIG)
lib_name := libxcore.a 
lib_path = $(config_dir)/$(lib_name)

.PHONY: libxcore
libxcore: $(lib_path)

.PHONY: clean
clean:
	rm -rf $(build_dir)

include $(wildcard $(build_dir)/*/*.d)

objects := \
	xcore_c_resource.o \
	xcore_c_inlines.o \
	xcore_c_interrupt.o \
	xcore_c_select.o \
	xcore_c_thread.o


$(build_dir)/%_release/$(lib_name) : config_flags := -DNDEBUG -O3
$(build_dir)/%_debug/$(lib_name) : config_flags := -O1

$(build_dir)/xs1b_%/$(lib_name) : config_march := xs1b
$(build_dir)/xs2a_%/$(lib_name) : config_march := xs2a
$(build_dir)/xs3a_%/$(lib_name) : config_march := xs3a



dependency_file_flag := -MD 

$(build_dir):
	mkdir $@

$(config_dir)/libxcore.a : $(addprefix $(config_dir)/,$(objects)) | $(config_dir)
	$(AR) rcs $@ $^

$(config_dir) : | $(build_dir)
	mkdir $@

.SECONDARY: $(config_dir)/%.o
$(config_dir)/%.o : %.c | $(config_dir)
	$(C_COMPILER) -c $< -o $@ --target=xcore -march=$(config_march) $(config_flags) $(addprefix -I,$(lib_incdirs)) $(global_compiler_flags) $(dependency_file_flag) -MF $|/$*.d $(addprefix -isystem,$(system_include_dirs))

.SECONDARY: $(config_dir)/%.o
$(config_dir)/%.o : %.S | $(config_dir)
	$(ASSEMBLER) -c $< -o $@ --target=xcore -march=$(config_march) $(config_flags) $(addprefix -I,$(lib_incdirs)) $(dependency_file_flag) -MF $|/$*.d


