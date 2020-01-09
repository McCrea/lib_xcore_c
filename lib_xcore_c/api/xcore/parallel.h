#pragma once

#include <xcore/_support/xcore_c_parallel_impl.h>

#define PAR_FUNCS(...) _XMM_PAR_FUNCS_I(__VA_ARGS__)
#define JOB(FUNCTION, ARGUMENT, STACK_SIZE) (FUNCTION, ARGUMENT, STACK_SIZE)
