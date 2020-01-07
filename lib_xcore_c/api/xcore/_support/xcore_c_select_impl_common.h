// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_select_impl_h__
#define __xcore_c_select_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <xcore/_support/xcore_c_macros.h>
#include <xcore/_support/xcore_c_meta_macro.h>
#include <xcore/_support/xcore_c_resource_impl.h>


inline void _select_disable_trigger_all(void)
{
  asm volatile("clre");
}

inline void _select_event_enable_unconditional(const resource_t resource)
{
  asm volatile(
    "eeu res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource));
}

inline void _select_event_disable_unconditional(const resource_t resource)
{
  asm volatile(
    "edu res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource));
}

inline void _select_event_set_enable(const resource_t resource, const int condition)
{
  asm volatile(
    "eet %[cond], res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource) , [cond] "r"(condition));
}

inline void _select_event_set_enable_inv(const resource_t resource, const int condition)
{
  asm volatile(
    "eef %[cond], res[%[res]] \n"
    : /* No outputs */
    : [res] "r"(resource) , [cond] "r"(condition));
}

inline void _select_event_enable_if_true(const resource_t resource, const int condition)
{
  if (__builtin_constant_p(condition))
  {
    if (condition)
    {
      _select_event_enable_unconditional(resource);
    }
    else
    {
      _select_event_disable_unconditional(resource);
    }
  }
  else 
  {
    _select_event_set_enable(resource, condition);
  }
  return condition;
}

inline void _select_event_enable_if_false(const resource_t resource, const int condition)
{
  if (__builtin_constant_p(condition))
  {
    if (condition)
    {
      _select_event_disable_unconditional(resource);
    }
    else
    {
      _select_event_enable_unconditional(resource);
    }
  }
  else 
  {
    _select_event_set_enable_inv(resource, condition);
  }
  return condition;
}

inline void _register_event_vector(const resource_t resource, void * const vector)
{
  register void * const vector_reg asm("r11") = vector; 
  asm volatile (
    "setv res[%[res]], %[target] \n"
    :
    : [res] "r"(resource), [target] "r"(vector_reg));
}

#define _XMM_GUARD_NONE _XMM_GTYPE_NONE, /* Guard expression should never be used */
#define _XMM_GUARD_TRUE(EXPR) _XMM_GTYPE_TRUE, EXPR
#define _XMM_GUARD_FALSE(EXPR) _XMM_GTYPE_FALSE, EXPR

#endif // !defined(__XC__)

#endif // __xcore_c_select_impl_h__

