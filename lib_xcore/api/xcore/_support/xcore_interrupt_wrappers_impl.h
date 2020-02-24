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

#define _XCORE_INTERRUPT_PERMITTED(root_function) \
    __xcore_interrupt_permitted_ugs_ ## root_function

#define _XCORE_DECLARE_INTERRUPT_PERMITTED(ret, root_function, ...) \
    void _XCORE_INTERRUPT_PERMITTED(root_function) (__VA_ARGS__);\
    ret root_function(__VA_ARGS__)

#define _XCORE_DEFINE_INTERRUPT_PERMITTED_DEF(grp, root_function) \
    .weak  _fptrgroup.grp.nstackwords.group; \
    .max_reduce _fptrgroup.grp.nstackwords, _fptrgroup.grp.nstackwords.group, 0; \
    .set _kstack_words, _XCORE_STACK_ALIGN(_fptrgroup.grp.nstackwords $M LIBXCORE_KSTACK_WORDS); \
    .globl __xcore_interrupt_permitted_common; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function); \
    .align _XCORE_CODE_ALIGNMENT; \
    .type  _XCORE_INTERRUPT_PERMITTED(root_function),@function; \
    .cc_top _XCORE_INTERRUPT_PERMITTED(root_function).function,_XCORE_INTERRUPT_PERMITTED(root_function); \
    _XCORE_INTERRUPT_PERMITTED(root_function):; \
      _XCORE_ENTSP(_XCORE_STACK_ALIGN(3)); \
      stw r5, sp[2]; \
      stw r4, sp[1]; \
      ldc r4, _kstack_words; \
      ldap r11, root_function; \
      add r5, r11, 0; \
      ldap r11, __xcore_interrupt_permitted_common; \
      bau r11; \
    .cc_bottom _XCORE_INTERRUPT_PERMITTED(root_function).function; \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).nstackwords, _XCORE_STACK_ALIGN(3) + _kstack_words + __xcore_interrupt_permitted_common.nstackwords + root_function.nstackwords; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).nstackwords; \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).maxcores, 1 $M __xcore_interrupt_permitted_common.maxcores $M root_function.maxcores; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).maxcores; \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).maxtimers, 0 $M __xcore_interrupt_permitted_common.maxtimers $M root_function.maxtimers; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).maxtimers; \
    .set   _XCORE_INTERRUPT_PERMITTED(root_function).maxchanends, 0 $M __xcore_select_callback_common.maxchanends $M root_function.maxchanends; \
    .globl _XCORE_INTERRUPT_PERMITTED(root_function).maxchanends; \
    .size  _XCORE_INTERRUPT_PERMITTED(root_function), . - _XCORE_INTERRUPT_PERMITTED(root_function); \

#define _XCORE_DEFINE_INTERRUPT_PERMITTED(grp, ret, root_function, ...) \
    asm(_XCORE_STR(_XCORE_DEFINE_INTERRUPT_PERMITTED_DEF(grp, root_function))); \
    _XCORE_DECLARE_INTERRUPT_PERMITTED(ret, root_function, __VA_ARGS__)


#define _XCORE_INTERRUPT_CALLBACK(intrpt) \
    __xcore_interrupt_callback_ ## intrpt

#define _XCORE_DECLARE_INTERRUPT_CALLBACK(intrpt, data) \
    void _XCORE_INTERRUPT_CALLBACK(intrpt)(void);\
    void intrpt(void* data)

#define _XCORE_DEFINE_INTERRUPT_CALLBACK_DEF(grp, intrpt) \
    .globl __xcore_interrupt_callback_common; \
    .weak _fptrgroup.grp.nstackwords.group; \
    .add_to_set _fptrgroup.grp.nstackwords.group, _XCORE_INTERRUPT_CALLBACK(intrpt).nstackwords, _XCORE_INTERRUPT_CALLBACK(intrpt); \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt); \
    .align _XCORE_CODE_ALIGNMENT; \
    .type  _XCORE_INTERRUPT_CALLBACK(intrpt),@function; \
    .cc_top _XCORE_INTERRUPT_CALLBACK(intrpt).function,_XCORE_INTERRUPT_CALLBACK(intrpt); \
    _XCORE_INTERRUPT_CALLBACK(intrpt):; \
      _XCORE_SINGLE_ISSUE /* Do we know what KEDI is set to? */; \
      kentsp _XCORE_STACK_ALIGN(7); \
      stw r11, sp[5]; \
      stw r1, sp[3]; \
      ldap r11, intrpt; \
      add r1, r11, 0; \
      ldap r11, __xcore_interrupt_callback_common; \
      bau r11; \
    .cc_bottom _XCORE_INTERRUPT_CALLBACK(intrpt).function; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).nstackwords, _XCORE_STACK_ALIGN(7) + __xcore_interrupt_callback_common.nstackwords + intrpt.nstackwords; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).nstackwords; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).maxcores, 1 $M __xcore_interrupt_callback_common.maxcores $M intrpt.maxcores; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).maxcores; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).maxtimers, 0 $M __xcore_interrupt_callback_common.maxtimers $M intrpt.maxtimers; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).maxtimers; \
    .set   _XCORE_INTERRUPT_CALLBACK(intrpt).maxchanends, 0 $M __xcore_interrupt_callback_common.maxchanends $M intrpt.maxchanends; \
    .globl _XCORE_INTERRUPT_CALLBACK(intrpt).maxchanends; \
    .size  _XCORE_INTERRUPT_CALLBACK(intrpt), . - _XCORE_INTERRUPT_CALLBACK(intrpt); \

#define _XCORE_DEFINE_INTERRUPT_CALLBACK(grp, intrpt, data) \
    asm(_XCORE_STR(_XCORE_DEFINE_INTERRUPT_CALLBACK_DEF(grp, intrpt))); \
    _XCORE_DECLARE_INTERRUPT_CALLBACK(intrpt, data)


#ifdef __cplusplus
}
#endif

