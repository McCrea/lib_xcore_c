// Copyright (c) 2020, XMOS Ltd, All rights reserved
#pragma once

/** \file
 * \brief Helper macros for fork/join style parallel execution
 */

#include <xcore/_support/xcore_c_parallel_impl.h>

/** \brief Calls a list of <tt>void (void*)</tt> functions with each utilising a different hardware thread
 *
 * Expands to a block which evaluates each of the given function call descriptions such that each
 * runs in a different thread. For each \c PFUNC, a stack of \c STACK_SIZE words will be allocated
 * from the current stack and used to call \c FUNCTION with \c ARGUMENT as its sole argument.
 * The block is blocking until all function calls have returned.
 * \note One function call will be evaluated in the current thread.
 *
 * \param ...  Functions to call, each the result of expanding \c PFUNC
 *
 * Example: \code
 * PAR_FUNCS(PFUNC(my_print_function, "Hello 1", 256),
 *           PFUNC(my_print_function, "Hello 2", 256));
 * \endcode
 * \see PFUNC
 * \hideinitializer
 */
#define PAR_FUNCS(...) _XMM_PAR_FUNCS_I(__VA_ARGS__)

/** \brief Specifies a parallelised function call
 *
 * Expands to a function call desciption which can be used as an argument to PAR_FUNCS.
 *  \param FUNCTION    a function with siganture <tt>void (void *)</tt>
 *  \param ARGUMENT    the argument to pass to \a FUNCTION - must be implicitly convertible to <tt>void *</tt>
 *  \param STACK_SIZE  stack size requirement of \a FUNCTION in words
 * \see PAR_FUNCS
 * \hideinitializer
 */
#define PFUNC(FUNCTION, ARGUMENT, STACK_SIZE) (FUNCTION, ARGUMENT, STACK_SIZE)

/** \brief Declare a \c void function with arbitrary functions which can be dispatched in another thread
 *
 * Expands to the declaration of a function named \a NAME and additional definitions required to
 * execute that function in a different thread.
 * \param NAME            Name of function to declare
 * \param ARG_TYPES_PACK  A pack of types which form the parameter part of the function 
                          signature (must not include names)
 * \param SSIZE           The stack size requirement of the function in words
 * 
 * Example: \code
 * DECLARE_JOB(thread_sum, (const unsigned *, size_t, unsigned long *), 128);
 * void thread_sum(const unsigned * const ints, const size_t number, unsigned long * const result) {
 *   unsigned long sum = 0;
 *   for (size_t i = 0; i < number; i += 1) { sum += ints[i]; }
 *   *result = sum;
 * }
 * \endcode
 * \see \li PJOB \li PAR_JOBS
 * \hideinitializer
 */
#define DECLARE_JOB(NAME, ARG_TYPES_PACK, SSIZE) \
  DECLARE_JOB_I(NAME, \
                _XMM_PAR_ARG_STRUCT_NAME(NAME), \
                _XMM_PAR_ARG_PACK_PREPARE(ARG_TYPES_PACK), \
                ARG_TYPES_PACK, \
                SSIZE)

/** \brief Calls a list of functions declared using DECLARE_JOB in parallel
 *
 * Expands to a block which executes each of the given function call specifications such that each
 * call takes place in a different hardward thread. Each call specification should be the result of
 * expanding PJOB and the \c FUNCTION should be one which was declared using DECLARE_JOB.
 * Each thread's stack will be \c SSIZE words (as passed to DECLARE_JOB) in size and will be allocated from
 * the current stack. Each function will execute as if called \c FUNCTION \c ARGPACK; had been executed in
 * its assigned thread.
 * The block is blocking until all functions have returned.
 *
 * \note One function call will be evaluated in the current thread.
 *
 * \param ...  Function call specifications as expanded from PJOB.
 *
 * Example: \code
 * unsigned long sum1, sum2;
 * PAR_JOBS(PJOB(thread_sum, (ints1, ints1_length, &sum1)),
 *          PJOB(thread_sum, (ints2, ints2_length, &sum2)));
 * \endcode
 * \see \li PJOB \li DECLARE_JOB
 * \hideinitializer
 */
#define PAR_JOBS(...) _XMM_JPAR_JOBS_I(__VA_ARGS__)

/** \brief Specifies a parallelised call of a function declared with DECLARE_JOB
 *
 * Expands to a function call desciption which can be used as an argument to PAR_JOBS.
 *  \param FUNCTION  a function which was declared using DECLARE_JOB
 *  \param ARGPACK   pack of the arguments to pass to \a FUNCTION - each must be implicitly
 *                   convertible to its respective parameter type.
 *                   E.g. <tt>(5, NULL)</tt> for <tt>void(int, int *)</tt>
 * \see \li PAR_JOBS \li DECLARE_JOB
 * \hideinitializer
 */
#define PJOB(FUNCTION, ARGPACK) (FUNCTION, ARGPACK)
