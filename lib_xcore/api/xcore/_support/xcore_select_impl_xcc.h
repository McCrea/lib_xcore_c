// Copyright (c) 2019-2020, XMOS Ltd, All rights reserved
#pragma once

#include <limits.h>
#include <xcore/_support/xcore_common.h>
#include <xcore/_support/xcore_select_impl_common.h>


#ifdef __cplusplus
extern "C" {
#endif


_XCORE_EXFUN
inline void __xcore_select_setup_int(resource_t r, uint32_t value)
{
  __xcore_resource_setup_callback(r, (void*)value, __xcore_select_non_callback, 0x2);
}


#define _XCORE_SELECT_RES_HANDLER_SETUP_I(RES, LABEL, ...) \
  do { \
    __xcore_select_setup_int(RES, __xcore_htable_idx); \
    __xcore_resource_event_enable_unconditional(RES); \
    __xcore_htable_idx += 1; \
  } while(0);

#define _XCORE_SELECT_RES_HANDLER_SETUP(PACK) _XCORE_PSHIM(_XCORE_SELECT_RES_HANDLER_SETUP_I, PACK)


#define _XCORE_SELECT_RES_ENABLER_ONEOFF__XCORE_GTYPE_NONE(RES, EXPR_) __xcore_resource_event_enable_unconditional(RES);

#define _XCORE_SELECT_RES_ENABLER_ONEOFF__XCORE_GTYPE_TRUE(...) /* None */
#define _XCORE_SELECT_RES_ENABLER_ONEOFF__XCORE_GTYPE_FALSE(...) /* None */

#define _XCORE_SELECT_RES_ENABLER_REPEAT__XCORE_GTYPE_NONE(...) /* None */

#define _XCORE_SELECT_RES_ENABLER_REPEAT__XCORE_GTYPE_TRUE(RES, COND) __xcore_resource_event_enable_if_true(RES, COND);
#define _XCORE_SELECT_RES_ENABLER_REPEAT__XCORE_GTYPE_FALSE(RES, COND) __xcore_resource_event_enable_if_false(RES, COND);

#define _XCORE_SELECT_RES_ENABLER_ONEOFF_NAME(GTYPE)  _XCORE_SELECT_RES_ENABLER_ONEOFF_ ## GTYPE
#define _XCORE_SELECT_RES_ENABLER_ONEOFF_I(RES, LABEL_, GTYPE, EXPR) _XCORE_SELECT_RES_ENABLER_ONEOFF_NAME(GTYPE)(RES, EXPR)
#define _XCORE_SELECT_RES_ENABLER_ONEOFF(PACK) _XCORE_PSHIM(_XCORE_SELECT_RES_ENABLER_ONEOFF_I, PACK)

#define _XCORE_SELECT_RES_ENABLER_REPEAT_NAME(GTYPE)  _XCORE_SELECT_RES_ENABLER_REPEAT_ ## GTYPE
#define _XCORE_SELECT_RES_ENABLER_REPEAT_I(RES, LABEL_, GTYPE, EXPR) _XCORE_SELECT_RES_ENABLER_REPEAT_NAME(GTYPE)(RES, EXPR)
#define _XCORE_SELECT_RES_ENABLER_REPEAT(PACK) _XCORE_PSHIM(_XCORE_SELECT_RES_ENABLER_REPEAT_I, PACK)

#define _XCORE_FILTER_SEL_DEFAULT__XCORE_SEL_RES(PACK_)
#define _XCORE_FILTER_SEL_DEFAULT__XCORE_SEL_DEFAULT(PACK) PACK,
#define _XCORE_FILTER_SEL_RES__XCORE_SEL_RES(PACK) , PACK
#define _XCORE_FILTER_SEL_RES__XCORE_SEL_DEFAULT(PACK)


#define _XCORE_FILTER_SEL_DEFAULT_NAME(SUFF) _XCORE_FILTER_SEL_DEFAULT_ ## SUFF
#define _XCORE_FILTER_SEL_DEFAULT_I(SUFF, PACK) _XCORE_FILTER_SEL_DEFAULT_NAME(SUFF)(PACK)
#define _XCORE_SELECT_RES_FILTER_DEFAULT_I(PACK) _XCORE_PSHIM(_XCORE_FILTER_SEL_DEFAULT_I, PACK)

#define _XCORE_FILTER_SEL_RES_NAME(SUFF) _XCORE_FILTER_SEL_RES_ ## SUFF 
#define _XCORE_FILTER_SEL_RES_I(SUFF, PACK) _XCORE_FILTER_SEL_RES_NAME(SUFF)(PACK)
#define _XCORE_SELECT_RES_FILTER_RES_I(PACK) _XCORE_PSHIM(_XCORE_FILTER_SEL_RES_I, PACK)

#define _XCORE_SELECT_DEFAULT_DEFAULT (, _XCORE_NO_DEFAULT_CASE, )
#define _XCORE_SELECT_RES_FILTER_DEFAULT(...) _XCORE_SHIM(_XCORE_HEAD, _XCORE_APPLY_NOSEP(_XCORE_SELECT_RES_FILTER_DEFAULT_I, __VA_ARGS__) _XCORE_SELECT_DEFAULT_DEFAULT)
#define _XCORE_SELECT_RES_FILTER_RES(...) _XCORE_SHIM(_XCORE_TAIL, _XCORE_APPLY_NOSEP(_XCORE_SELECT_RES_FILTER_RES_I, __VA_ARGS__))


#define _XCORE_LABEL_I(VAL_, LABEL, ...) LABEL
#define _XCORE_LABEL(PACK) _XCORE_PSHIM(_XCORE_LABEL_I, PACK)

#if defined(__XS1B__) || defined(__XS1C__)
#define RES_EV_FORCE_MASK 0x10000
#else
#define RES_EV_FORCE_MASK 0x0
#endif

#define _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(TNAME, ...) \
  do { \
    const int __xcore_wait_id  = select_no_wait(UINT_MAX); \
    if (__xcore_wait_id != UINT_MAX) \
    { \
      goto* TNAME[__xcore_wait_id - RES_EV_FORCE_MASK]; \
    } \
  } while (0)

#define _XCORE_SELECT_WAIT_HANDLER__XCORE_GTYPE_NONE(COND_, LABEL, TNAME, ...) \
  _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(TNAME, __VA_ARGS__); \
  goto LABEL;

#define _XCORE_SELECT_WAIT_HANDLER__XCORE_NO_DEFAULT_CASE(COND_, LABEL_, TNAME, ...) \
  goto* TNAME[select_wait() - RES_EV_FORCE_MASK];

#define _XCORE_SELECT_WAIT_HANDLER__XCORE_GTYPE_TRUE(COND, LABEL, TNAME, ...) \
  do { \
    _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(TNAME, __VA_ARGS__); \
    const int __xcore_wait_cond = COND; \
    if (__xcore_wait_cond) \
    { \
      goto LABEL; \
    } \
    else \
    { \
      goto* TNAME[select_wait() - RES_EV_FORCE_MASK]; \
    } \
  } while (0);

#define _XCORE_SELECT_WAIT_HANDLER__XCORE_GTYPE_FALSE(COND, LABEL, TNAME, ...) \
  do { \
    _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(TNAME, __VA_ARGS__); \
    const int __xcore_wait_cond = COND; \
    if (__xcore_wait_cond) \
    { \
      goto* TNAME[select_wait() - RES_EV_FORCE_MASK]; \
    } \
    else \
    { \
      goto LABEL; \
    } \
  } while (0);

#define _XCORE_SELECT_WAIT_HANDLER_NAME(SUFF) _XCORE_SELECT_WAIT_HANDLER_ ## SUFF
#define _XCORE_SELECT_WAIT_HANDLER_I(LABEL, TYPE, COND, TNAME, ...) _XCORE_SELECT_WAIT_HANDLER_NAME(TYPE)(COND, LABEL, TNAME, __VA_ARGS__)
#define _XCORE_SELECT_WAIT_HANDLER(DEFAULT_PACK, TNAME, ...) _XCORE_PSHIM(_XCORE_SELECT_WAIT_HANDLER_I, DEFAULT_PACK, TNAME, __VA_ARGS__)

#define _XCORE_SELECT_RES_I(TNAME, LNAME, DEFAULT_PACK, ...) \
  switch (0) for (const void *__xcore_select_reset;;) if (1) \
  { \
    static void *TNAME[] = { _XCORE_SHIM(_XCORE_APPLY, _XCORE_LABELADDR, _XCORE_APPLY(_XCORE_LABEL, __VA_ARGS__)) };\
    _XCORE_APPLY_NOSEP(_XCORE_SELECT_RES_ENABLER_REPEAT, __VA_ARGS__) \
    _XCORE_SELECT_WAIT_HANDLER(DEFAULT_PACK, TNAME, _XCORE_APPLY(_XCORE_LABEL, __VA_ARGS__)) \
  } \
  else if (0) \
  { \
  case 0: \
  default: \
    __xcore_select_reset = &&LNAME; \
  LNAME: \
    __xcore_select_disable_trigger_all(); \
    unsigned __xcore_htable_idx = RES_EV_FORCE_MASK; \
    _XCORE_APPLY_NOSEP(_XCORE_SELECT_RES_HANDLER_SETUP, __VA_ARGS__) \
    _XCORE_APPLY_NOSEP(_XCORE_SELECT_RES_ENABLER_ONEOFF, __VA_ARGS__) \
  } \
  else

#define _XCORE_SELECT_RES(...) _XCORE_SELECT_RES_I(_XCORE_UNIQUE_LABEL(__xcore_htable), _XCORE_UNIQUE_LABEL(__xcore_sel_reset), _XCORE_SELECT_RES_FILTER_DEFAULT(__VA_ARGS__), _XCORE_SELECT_RES_FILTER_RES(__VA_ARGS__))

#define _XCORE_SELECT_RES_ENABLER_ORDERED__XCORE_GTYPE_TRUE(RES, EXPR, TNAME, ...) \
  do { \
    register const int __xcore_enable_cond = EXPR; \
    __xcore_resource_event_enable_if_true(RES, __xcore_enable_cond); \
    /*if (__xcore_enable_cond) { _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); } */\
    _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(TNAME, __VA_ARGS__); \
  } while (0);

#define _XCORE_SELECT_RES_ENABLER_ORDERED__XCORE_GTYPE_FALSE(RES, EXPR, TNAME, ...) \
  do { \
    register const int __xcore_enable_cond = EXPR; \
    __xcore_resource_event_enable_if_false(RES, __xcore_enable_cond); \
    /* if (!__xcore_enable_cond) { _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); } */\
    _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(TNAME, __VA_ARGS__); \
  } while (0);

#define _XCORE_SELECT_RES_ENABLER_ORDERED__XCORE_GTYPE_NONE(RES, EXPR_, TNAME, ...) \
  __xcore_resource_event_enable_unconditional(RES); \
  _XCORE_SELECT_TAKE_EVENT_NONBLOCKING(TNAME, __VA_ARGS__);


#define _XCORE_SELECT_RES_ENABLER_ORDERED_NAME(GTYPE)  _XCORE_SELECT_RES_ENABLER_ORDERED_ ## GTYPE
#define _XCORE_SELECT_RES_ENABLER_ORDERED_III(RES, LABEL_, GTYPE, EXPR, TNAME, ...) _XCORE_SELECT_RES_ENABLER_ORDERED_NAME(GTYPE)(RES, EXPR, TNAME, __VA_ARGS__)
#define _XCORE_SELECT_RES_ENABLER_ORDERED_II(...) _XCORE_SELECT_RES_ENABLER_ORDERED_III(__VA_ARGS__)
#define _XCORE_SELECT_RES_ENABLER_ORDERED_I(LABELS, PACK) _XCORE_SELECT_RES_ENABLER_ORDERED_II(_XCORE_UNPACK(_XCORE_PACK_JOIN(PACK , LABELS)))
#define _XCORE_SELECT_RES_ENABLER_ORDERED(PACK) _XCORE_SELECT_RES_ENABLER_ORDERED_I PACK

#define _XCORE_SELECT_RES_ORDERED_II(TNAME, LNAME, DEFAULT_PACK, LABELS, ...) \
  switch (0) for (const void *__xcore_select_reset;;) if (1) \
  { \
    static void *TNAME[] = { _XCORE_SHIM(_XCORE_APPLY, _XCORE_LABELADDR, _XCORE_APPLY(_XCORE_LABEL, __VA_ARGS__)) }; \
    __xcore_select_disable_trigger_all(); \
    _XCORE_SHIM(_XCORE_APPLY_NOSEP, _XCORE_SELECT_RES_ENABLER_ORDERED, _XCORE_I(_XCORE_TAG(LABELS, __VA_ARGS__))) \
    _XCORE_SELECT_WAIT_HANDLER(DEFAULT_PACK, TNAME, _XCORE_APPLY(_XCORE_LABEL, __VA_ARGS__)) \
  } \
  else if (0) \
  { \
  case 0: \
  default: \
    __xcore_select_reset = &&LNAME; \
  LNAME: \
    __xcore_select_disable_trigger_all(); \
    unsigned __xcore_htable_idx = RES_EV_FORCE_MASK; \
    _XCORE_APPLY_NOSEP(_XCORE_SELECT_RES_HANDLER_SETUP, __VA_ARGS__) \
  } \
  else

// Note: TNAME gets passed as first label name for convenience
#define _XCORE_SELECT_RES_ORDERED_I(TNAME, LNAME, DEFAULT_PACK, ...) _XCORE_SELECT_RES_ORDERED_II(TNAME, LNAME, DEFAULT_PACK, (TNAME, _XCORE_APPLY(_XCORE_LABEL, __VA_ARGS__)), __VA_ARGS__)

#define _XCORE_SELECT_RES_ORDERED(...) \
  _XCORE_SELECT_RES_ORDERED_I( \
    _XCORE_UNIQUE_LABEL(__xcore_htable), \
    _XCORE_UNIQUE_LABEL(__xcore_sel_reset), \
    _XCORE_SELECT_RES_FILTER_DEFAULT(__VA_ARGS__), \
    _XCORE_SELECT_RES_FILTER_RES(__VA_ARGS__))


#define _XCORE_SELECT_RESET_I do { goto* __xcore_select_reset; } while (0)

#define _XCORE_CASE_RES(...) (_XCORE_SEL_RES, (__VA_ARGS__))
#define _XCORE_CASE_DEFAULT(...) (_XCORE_SEL_DEFAULT, (__VA_ARGS__))

#ifdef __cplusplus
}
#endif

