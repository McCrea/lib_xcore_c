// Copyright (c) 2016-2020, XMOS Ltd, All rights reserved
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Prefix for inline function defintions in headers.
// This can be defined as extern to allow building a library from the header definitions.
#ifndef _XCORE_EXFUN
#define _XCORE_EXFUN
#endif

#ifdef __cplusplus
#define _XCORE_CFUNC_EXCEPT_SPEC noexcept
#else
#define _XCORE_CFUNC_EXCEPT_SPEC
#endif
#define _XCORE_NOTHROW _XCORE_CFUNC_EXCEPT_SPEC

typedef unsigned resource_t;

#ifdef __cplusplus
}
#endif

