// Copyright (c) 2020, XMOS Ltd, All rights reserved

#ifndef _XCORE_EXFUN
// TODO: When using Clang we make all inlines static as a workaround... Remove this once that's fixed!
#ifdef XCC_VERSION_MAJOR
#define _XCORE_EXFUN //extern
#endif
#endif

#include <xcore/_support/xcore_c_all.h>
