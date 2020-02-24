// Copyright (c) 2020, XMOS Ltd, All rights reserved

#include <xs1.h>

#ifdef __cplusplus
extern "C" {
#endif


#if defined(XCC_VERSION_MAJOR) || defined(NO_XCLANG_ADDITIONS)

#include <xcore/_support/xcore_resource_impl.h>

#define _XCORE_TRIGGERABLE_SETUP_EVENT_VECTOR(RESOURCE, LABEL) \
  do { __xcore_resource_setup_callback((RESOURCE), (&&LABEL), __xcore_select_non_callback, 0x2); } while (0)

#define _XCORE_TRIGGERABLE_WAIT_EVENT(...) \
  do { goto* (void*)select_wait(); } while(0)

#define _XCORE_TRIGGERABLE_TAKE_EVENT(...) \
  do { \
    void * const __xmm_triggerable_vector = select_no_wait(NULL); \
    if (__xmm_triggerable_vector != NULL) { \
      goto* __xmm_triggerable_vector; \
    } \
   } while(0)

#else

#define _XCORE_TRIGGERABLE_SETUP_EVENT_VECTOR(RESOURCE, LABEL) \
  do { __xcore_resource_register_event_vector((RESOURCE), (&&LABEL)); } while (0)

#define _XCORE_TRIGGERABLE_WAIT_EVENT(LABELS...) \
  do { \
    asm goto("waiteu" \
      : /* Can't have outputs */ \
      : /* No inputs */ \
      : /* No clobbers */ \
      : LABELS); \
    __builtin_unreachable(); \
  } while (0)

#define _XCORE_TRIGGERABLE_TAKE_EVENT(LABELS...) \
  do { \
    asm goto("setsr %[mask] \n" \
             "clrsr %[mask] \n" \
             : /* Can't have outputs */ \
             : [mask] "n"(XS1_SR_EEBLE_MASK) \
             : /* No clobbers */ \
             : LABELS); \
  } while (0)

#endif

_XCORE_EXFUN
inline void __triggerable_disable_all(void)
{
  asm volatile("clre");
}

#ifdef __cplusplus
}
#endif

