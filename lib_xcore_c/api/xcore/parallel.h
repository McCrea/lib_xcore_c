#pragma once

#include <xcore/_support/xcore_c_parallel_impl.h>

#define PAR_FUNCS(...) _XMM_PAR_FUNCS_I(__VA_ARGS__)
#define PFUNC(FUNCTION, ARGUMENT, STACK_SIZE) (FUNCTION, ARGUMENT, STACK_SIZE)

#define DECLARE_JOB(NAME, ARG_TYPES_PACK, SSIZE) \
  DECLARE_JOB_I(NAME, \
                _XMM_PAR_ARG_STRUCT_NAME(NAME), \
                _XMM_PAR_ARG_PACK_PREPARE(ARG_TYPES_PACK), \
                ARG_TYPES_PACK, \
                SSIZE)

#define PAR_JOBS(...) _XMM_JPAR_JOBS_I(__VA_ARGS__)
#define PJOB(FUNC, ARGPACK) (FUNC, ARGPACK)
