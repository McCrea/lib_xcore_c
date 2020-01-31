// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once


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


