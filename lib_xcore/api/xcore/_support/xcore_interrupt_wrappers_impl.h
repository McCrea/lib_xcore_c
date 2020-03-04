// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_macros.h>
#include <xcore/_support/xcore_resource_impl.h>


#ifdef __cplusplus
extern "C" {
#endif


// The user may define a larger kstack for their own worse case use.
#ifndef LIBXCORE_KSTACK_WORDS
#define LIBXCORE_KSTACK_WORDS 0
#endif

#define _XCORE_INTERRUPT_PERMITTED(__root_function) \
    __xcore_interrupt_permitted_ugs_ ## __root_function

#define _XCORE_DECLARE_INTERRUPT_PERMITTED(__ret, __root_function, ...) \
    void _XCORE_INTERRUPT_PERMITTED(__root_function) (__VA_ARGS__);\
    __ret __root_function(__VA_ARGS__)

#define _XCORE_DEFINE_INTERRUPT_PERMITTED_DEF(__grp, __root_function) \
    .weak  _fptrgroup.__grp.nstackwords.group; \
    .max_reduce _fptrgroup.__grp.nstackwords, _fptrgroup.__grp.nstackwords.group, 0; \
    .set _kstack_words, _XCORE_STACK_ALIGN(_fptrgroup.__grp.nstackwords $M LIBXCORE_KSTACK_WORDS); \
    .globl __xcore_interrupt_permitted_common; \
    .globl _XCORE_INTERRUPT_PERMITTED(__root_function); \
    .align _XCORE_CODE_ALIGNMENT; \
    .type  _XCORE_INTERRUPT_PERMITTED(__root_function),@function; \
    .cc_top _XCORE_INTERRUPT_PERMITTED(__root_function).function,_XCORE_INTERRUPT_PERMITTED(__root_function); \
    _XCORE_INTERRUPT_PERMITTED(__root_function):; \
      _XCORE_ENTSP(_XCORE_STACK_ALIGN(3)); \
      stw r5, sp[2]; \
      stw r4, sp[1]; \
      ldc r4, _kstack_words; \
      ldap r11, __root_function; \
      add r5, r11, 0; \
      ldap r11, __xcore_interrupt_permitted_common; \
      bau r11; \
    .cc_bottom _XCORE_INTERRUPT_PERMITTED(__root_function).function; \
    .set   _XCORE_INTERRUPT_PERMITTED(__root_function).nstackwords, _XCORE_STACK_ALIGN(3) + _kstack_words + __xcore_interrupt_permitted_common.nstackwords + __root_function.nstackwords; \
    .globl _XCORE_INTERRUPT_PERMITTED(__root_function).nstackwords; \
    .set   _XCORE_INTERRUPT_PERMITTED(__root_function).maxcores, 1 $M __xcore_interrupt_permitted_common.maxcores $M __root_function.maxcores; \
    .globl _XCORE_INTERRUPT_PERMITTED(__root_function).maxcores; \
    .set   _XCORE_INTERRUPT_PERMITTED(__root_function).maxtimers, 0 $M __xcore_interrupt_permitted_common.maxtimers $M __root_function.maxtimers; \
    .globl _XCORE_INTERRUPT_PERMITTED(__root_function).maxtimers; \
    .set   _XCORE_INTERRUPT_PERMITTED(__root_function).maxchanends, 0 $M __xcore_select_callback_common.maxchanends $M __root_function.maxchanends; \
    .globl _XCORE_INTERRUPT_PERMITTED(__root_function).maxchanends; \
    .size  _XCORE_INTERRUPT_PERMITTED(__root_function), . - _XCORE_INTERRUPT_PERMITTED(__root_function); \

#define _XCORE_DEFINE_INTERRUPT_PERMITTED(__grp, __ret, __root_function, ...) \
    asm(_XCORE_STRINGIFY(_XCORE_DEFINE_INTERRUPT_PERMITTED_DEF(__grp, __root_function))); \
    _XCORE_DECLARE_INTERRUPT_PERMITTED(__ret, __root_function, __VA_ARGS__)


#define _XCORE_INTERRUPT_CALLBACK(__intrpt) \
    __xcore_interrupt_callback_ ## __intrpt

#define _XCORE_DECLARE_INTERRUPT_CALLBACK(__intrpt, __data) \
    void _XCORE_INTERRUPT_CALLBACK(__intrpt)(void);\
    void __intrpt(void* __data)

#define _XCORE_DEFINE_INTERRUPT_CALLBACK_DEF(__grp, __intrpt) \
    .globl __xcore_interrupt_callback_common; \
    .weak _fptrgroup.__grp.nstackwords.group; \
    .add_to_set _fptrgroup.__grp.nstackwords.group, _XCORE_INTERRUPT_CALLBACK(__intrpt).nstackwords, _XCORE_INTERRUPT_CALLBACK(__intrpt); \
    .globl _XCORE_INTERRUPT_CALLBACK(__intrpt); \
    .align _XCORE_CODE_ALIGNMENT; \
    .type  _XCORE_INTERRUPT_CALLBACK(__intrpt),@function; \
    .cc_top _XCORE_INTERRUPT_CALLBACK(__intrpt).function,_XCORE_INTERRUPT_CALLBACK(__intrpt); \
    _XCORE_INTERRUPT_CALLBACK(__intrpt):; \
      _XCORE_SINGLE_ISSUE /* Do we know what KEDI is set to? */; \
      kentsp _XCORE_STACK_ALIGN(7); \
      stw r11, sp[5]; \
      stw r1, sp[3]; \
      ldap r11, __intrpt; \
      add r1, r11, 0; \
      ldap r11, __xcore_interrupt_callback_common; \
      bau r11; \
    .cc_bottom _XCORE_INTERRUPT_CALLBACK(__intrpt).function; \
    .set   _XCORE_INTERRUPT_CALLBACK(__intrpt).nstackwords, _XCORE_STACK_ALIGN(7) + __xcore_interrupt_callback_common.nstackwords + __intrpt.nstackwords; \
    .globl _XCORE_INTERRUPT_CALLBACK(__intrpt).nstackwords; \
    .set   _XCORE_INTERRUPT_CALLBACK(__intrpt).maxcores, 1 $M __xcore_interrupt_callback_common.maxcores $M __intrpt.maxcores; \
    .globl _XCORE_INTERRUPT_CALLBACK(__intrpt).maxcores; \
    .set   _XCORE_INTERRUPT_CALLBACK(__intrpt).maxtimers, 0 $M __xcore_interrupt_callback_common.maxtimers $M __intrpt.maxtimers; \
    .globl _XCORE_INTERRUPT_CALLBACK(__intrpt).maxtimers; \
    .set   _XCORE_INTERRUPT_CALLBACK(__intrpt).maxchanends, 0 $M __xcore_interrupt_callback_common.maxchanends $M __intrpt.maxchanends; \
    .globl _XCORE_INTERRUPT_CALLBACK(__intrpt).maxchanends; \
    .size  _XCORE_INTERRUPT_CALLBACK(__intrpt), . - _XCORE_INTERRUPT_CALLBACK(__intrpt); \

#define _XCORE_DEFINE_INTERRUPT_CALLBACK(__grp, __intrpt, __data) \
    asm(_XCORE_STRINGIFY(_XCORE_DEFINE_INTERRUPT_CALLBACK_DEF(__grp, __intrpt))); \
    _XCORE_DECLARE_INTERRUPT_CALLBACK(__intrpt, __data)


#ifdef __cplusplus
}
#endif

