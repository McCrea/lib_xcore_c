// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

#include <platform.h>

#ifdef __cplusplus
extern "C" {
#endif


#if !defined(__XS1B__) && !defined(__XS1C__) && !defined(__XS2A__) && !defined(__XS3A__)
#error Unsupported platform!
#endif

// Dual Issue
#if defined(__XS1B__) || defined(__XS1C__)
#else
#define _XCORE_HAS_DUAL_ISSUE
#endif

// Reference Clock
#if defined(__XS1B__) || defined(__XS1C__)
#else
#define _XCORE_HAS_REFERENCE_CLOCK
#endif

#ifdef __cplusplus
}
#endif

