#pragma once

#include <xcore/_support/xcore_c_select_impl_common.h>

#define _XMM_SELECT_RES_HANDLER_SETUP_I(RES, LABEL, ...) _register_event_vector(RES, &&LABEL);

#define _XMM_SELECT_RES_HANDLER_SETUP(PACK) _XMM_PSHIM(_XMM_SELECT_RES_HANDLER_SETUP_I, PACK)


#define _XMM_SELECT_RES_ENABLER_ONEOFF__XMM_GTYPE_NONE(RES, EXPR_) _select_event_enable_unconditional(RES);

#define _XMM_SELECT_RES_ENABLER_ONEOFF__XMM_GTYPE_TRUE(...) /* None */
#define _XMM_SELECT_RES_ENABLER_ONEOFF__XMM_GTYPE_FALSE(...) /* None */

#define _XMM_SELECT_RES_ENABLER_REPEAT__XMM_GTYPE_NONE(...) /* None */

#define _XMM_SELECT_RES_ENABLER_REPEAT__XMM_GTYPE_TRUE(RES, COND) _select_event_enable_if_true(RES, COND);
#define _XMM_SELECT_RES_ENABLER_REPEAT__XMM_GTYPE_FALSE(RES, COND) _select_event_enable_if_false(RES, COND);

#define _XMM_SELECT_RES_ENABLER_ONEOFF_NAME(GTYPE)  _XMM_SELECT_RES_ENABLER_ONEOFF_ ## GTYPE
#define _XMM_SELECT_RES_ENABLER_ONEOFF_I(RES, LABEL_, GTYPE, EXPR) _XMM_SELECT_RES_ENABLER_ONEOFF_NAME(GTYPE)(RES, EXPR)
#define _XMM_SELECT_RES_ENABLER_ONEOFF(PACK) _XMM_PSHIM(_XMM_SELECT_RES_ENABLER_ONEOFF_I, PACK)

#define _XMM_SELECT_RES_ENABLER_REPEAT_NAME(GTYPE)  _XMM_SELECT_RES_ENABLER_REPEAT_ ## GTYPE
#define _XMM_SELECT_RES_ENABLER_REPEAT_I(RES, LABEL_, GTYPE, EXPR) _XMM_SELECT_RES_ENABLER_REPEAT_NAME(GTYPE)(RES, EXPR)
#define _XMM_SELECT_RES_ENABLER_REPEAT(PACK) _XMM_PSHIM(_XMM_SELECT_RES_ENABLER_REPEAT_I, PACK)

#define _XMM_FILTER_SEL_DEFAULT__XMM_SEL_RES(PACK_)
#define _XMM_FILTER_SEL_DEFAULT__XMM_SEL_DEFAULT(PACK) PACK,
#define _XMM_FILTER_SEL_RES__XMM_SEL_RES(PACK) , PACK
#define _XMM_FILTER_SEL_RES__XMM_SEL_DEFAULT(PACK)


#define _XMM_FILTER_SEL_DEFAULT_NAME(SUFF) _XMM_FILTER_SEL_DEFAULT_ ## SUFF
#define _XMM_FILTER_SEL_DEFAULT_I(SUFF, PACK) _XMM_FILTER_SEL_DEFAULT_NAME(SUFF)(PACK)
#define _XMM_SELECT_RES_FILTER_DEFAULT_I(PACK) _XMM_PSHIM(_XMM_FILTER_SEL_DEFAULT_I, PACK)

#define _XMM_FILTER_SEL_RES_NAME(SUFF) _XMM_FILTER_SEL_RES_ ## SUFF 
#define _XMM_FILTER_SEL_RES_I(SUFF, PACK) _XMM_FILTER_SEL_RES_NAME(SUFF)(PACK)
#define _XMM_SELECT_RES_FILTER_RES_I(PACK) _XMM_PSHIM(_XMM_FILTER_SEL_RES_I, PACK)

#define _XMM_SELECT_DEFAULT_DEFAULT (, _XMM_NO_DEFAULT_CASE, )
#define _XMM_SELECT_RES_FILTER_DEFAULT(...) _XMM_SHIM(_XMM_HEAD, _XMM_APPLY_NOSEP(_XMM_SELECT_RES_FILTER_DEFAULT_I, __VA_ARGS__) _XMM_SELECT_DEFAULT_DEFAULT)
#define _XMM_SELECT_RES_FILTER_RES(...) _XMM_SHIM(_XMM_TAIL, _XMM_APPLY_NOSEP(_XMM_SELECT_RES_FILTER_RES_I, __VA_ARGS__))


#define _XMM_LABEL_I(VAL_, LABEL, ...) LABEL
#define _XMM_LABEL(PACK) _XMM_PSHIM(_XMM_LABEL_I, PACK)


#define _XMM_SELECT_TAKE_EVENT_NONBLOCKING(...) \
  asm goto("setsr %[mask] \n" \
           "clrsr %[mask] \n" \
           : /* Can't have outputs */ \
           : [mask] "n"(XS1_SR_EEBLE_MASK) \
           : /* No clobbers */ \
           : __VA_ARGS__)

#define _XMM_SELECT_WAIT_HANDLER__XMM_GTYPE_NONE(COND_, LABEL, ...) \
  _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); \
  goto LABEL;


#define _XMM_SELECT_WAIT_HANDLER__XMM_NO_DEFAULT_CASE(COND_, LABEL_, ...) \
  asm goto("waiteu" \
    : /* Can't have outputs */ : /* No inputs */ : /* No clobbers */ \
    : __VA_ARGS__); \
  __builtin_unreachable();


#define _XMM_SELECT_WAIT_HANDLER__XMM_GTYPE_TRUE(COND, LABEL, ...) \
  do { \
    const int __xmm_wait_cond = COND; \
    if (__builtin_constant_p(__xmm_wait_cond)) { \
      if (__xmm_wait_cond) { _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); goto LABEL; } \
      else { _XMM_SELECT_WAIT_HANDLER__XMM_NO_DEFAULT_CASE(,, __VA_ARGS__) } \
    } else { \
      _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); \
      asm goto("waitef %[cond]" \
        : /* Can't have outputs */ \
        : [cond] "r"(__xmm_wait_cond) \
        : /* No clobbers */ \
        : __VA_ARGS__); \
      __builtin_assume(__xmm_wait_cond); \
      goto LABEL; \
    } \
  } while (0);

#define _XMM_SELECT_WAIT_HANDLER__XMM_GTYPE_FALSE(COND, LABEL, ...) \
  do { \
    const int __xmm_wait_cond = COND; \
    if (__builtin_constant_p(__xmm_wait_cond)) { \
      if (__xmm_wait_cond) { _XMM_SELECT_WAIT_HANDLER__XMM_NO_DEFAULT_CASE(,, __VA_ARGS__); } \
      else { _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); goto LABEL; } \
    } else { \
      _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); \
      asm goto("waitet %[cond]" \
        : /* Can't have outputs */ \
        : [cond] "r"(__xmm_wait_cond) \
        : /* No clobbers */ \
        : __VA_ARGS__); \
      __builtin_assume(!__xmm_wait_cond); \
      goto LABEL; \
    } \
  } while (0);

#define _XMM_SELECT_WAIT_HANDLER_NAME(SUFF) _XMM_SELECT_WAIT_HANDLER_ ## SUFF
#define _XMM_SELECT_WAIT_HANDLER_I(LABEL, TYPE, COND, ...) _XMM_SELECT_WAIT_HANDLER_NAME(TYPE)(COND, LABEL, __VA_ARGS__)
#define _XMM_SELECT_WAIT_HANDLER(DEFAULT_PACK, ...) _XMM_PSHIM(_XMM_SELECT_WAIT_HANDLER_I, DEFAULT_PACK, __VA_ARGS__)

#define _XMM_SELECT_RES_I(LNAME, DEFAULT_PACK, ...) \
  switch (0) for (const void *__xmm_select_reset;;) if (1) \
  { \
    _XMM_APPLY_NOSEP(_XMM_SELECT_RES_ENABLER_REPEAT, __VA_ARGS__) \
    _XMM_SELECT_WAIT_HANDLER(DEFAULT_PACK, _XMM_APPLY(_XMM_LABEL, __VA_ARGS__)) \
  } \
  else if (0) \
  { \
  case 0: \
  default: \
    __xmm_select_reset = &&LNAME; \
  LNAME: \
    asm volatile("clre"); \
    _XMM_APPLY_NOSEP(_XMM_SELECT_RES_HANDLER_SETUP, __VA_ARGS__) \
    _XMM_APPLY_NOSEP(_XMM_SELECT_RES_ENABLER_ONEOFF, __VA_ARGS__) \
  } \
  else

#define _XMM_SELECT_RES(...) _XMM_SELECT_RES_I(_XMM_UNIQUE_LABEL(__xmm_sel_reset), _XMM_SELECT_RES_FILTER_DEFAULT(__VA_ARGS__), _XMM_SELECT_RES_FILTER_RES(__VA_ARGS__))

#define _XMM_SELECT_RES_ENABLER_ORDERED__XMM_GTYPE_TRUE(RES, EXPR, ...) \
  do { \
    register const int __xmm_enable_cond = EXPR; \
    _select_event_enable_if_true(RES, __xmm_enable_cond); \
    /*if (__xmm_enable_cond) { _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); } */\
    _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); \
  } while (0);

#define _XMM_SELECT_RES_ENABLER_ORDERED__XMM_GTYPE_FALSE(RES, EXPR, ...) \
  do { \
    register const int __xmm_enable_cond = EXPR; \
    _select_event_enable_if_false(RES, __xmm_enable_cond); \
    /* if (!__xmm_enable_cond) { _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); } */\
    _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__); \
  } while (0);

#define _XMM_SELECT_RES_ENABLER_ORDERED__XMM_GTYPE_NONE(RES, EXPR_, ...) \
  _select_event_enable_unconditional(RES); \
  _XMM_SELECT_TAKE_EVENT_NONBLOCKING(__VA_ARGS__);


#define _XMM_SELECT_RES_ENABLER_ORDERED_NAME(GTYPE)  _XMM_SELECT_RES_ENABLER_ORDERED_ ## GTYPE
#define _XMM_SELECT_RES_ENABLER_ORDERED_III(RES, LABEL_, GTYPE, EXPR, ...) _XMM_SELECT_RES_ENABLER_ORDERED_NAME(GTYPE)(RES, EXPR, __VA_ARGS__)
#define _XMM_SELECT_RES_ENABLER_ORDERED_II(...) _XMM_SELECT_RES_ENABLER_ORDERED_III(__VA_ARGS__)
#define _XMM_SELECT_RES_ENABLER_ORDERED_I(LABELS, PACK) _XMM_SELECT_RES_ENABLER_ORDERED_II(_XMM_UNPACK(_XMM_PACK_JOIN(PACK , LABELS)))
#define _XMM_SELECT_RES_ENABLER_ORDERED(PACK) _XMM_SELECT_RES_ENABLER_ORDERED_I PACK

#define _XMM_SELECT_RES_ORDERED_II(LNAME, DEFAULT_PACK, LABELS, ...) \
  switch (0) for (const void *__xmm_select_reset;;) if (1) \
  { \
    asm volatile("clre"); \
    _XMM_SHIM(_XMM_APPLY_NOSEP, _XMM_SELECT_RES_ENABLER_ORDERED, _XMM_I(_XMM_TAG(LABELS, __VA_ARGS__))) \
    _XMM_SELECT_WAIT_HANDLER(DEFAULT_PACK, _XMM_APPLY(_XMM_LABEL, __VA_ARGS__)) \
  } \
  else if (0) \
  { \
  case 0: \
  default: \
    __xmm_select_reset = &&LNAME; \
  LNAME: \
    _XMM_APPLY_NOSEP(_XMM_SELECT_RES_HANDLER_SETUP, __VA_ARGS__) \
  } \
  else

#define _XMM_SELECT_RES_ORDERED_I(LNAME, DEFAULT_PACK, ...) _XMM_SELECT_RES_ORDERED_II(LNAME, DEFAULT_PACK, (_XMM_APPLY(_XMM_LABEL, __VA_ARGS__)), __VA_ARGS__)

#define _XMM_SELECT_RES_ORDERED(...) \
  _XMM_SELECT_RES_ORDERED_I( \
    _XMM_UNIQUE_LABEL(__xmm_sel_reset), \
    _XMM_SELECT_RES_FILTER_DEFAULT(__VA_ARGS__), \
    _XMM_SELECT_RES_FILTER_RES(__VA_ARGS__))


#define _XMM_SELECT_RES(...) (_XMM_SEL_RES, (__VA_ARGS__))
#define _XMM_SELECT_DEFAULT(...) (_XMM_SEL_DEFAULT, (__VA_ARGS__))

#define _XMM_SELECT_RESET_I do { goto* __xmm_select_reset; } while (0)

