#include <xcore/thread.h>
#include <stdio.h>

void say(void *str)
{
  puts((const char *)str);
}

static const unsigned stack_words = 512;
static const unsigned stack_bytes = stack_words * sizeof(long);
#define STACK_ALIGN 4

int say_hello_using_xthreads(void)
{
  char stack_0[stack_bytes] __attribute__ ((aligned (STACK_ALIGN)));
  char stack_1[stack_bytes] __attribute__ ((aligned (STACK_ALIGN)));

  xthread_t thread_0 = xthread_alloc_and_start(say, "(XT1) Hello world!", stack_base(stack_0, stack_words)),
            thread_1 = xthread_alloc_and_start(say, "(XT2) Hello world!", stack_base(stack_1, stack_words));

  say("(XT Master) Hello world!");

  xthread_wait_and_free(thread_0);
  xthread_wait_and_free(thread_1);
}

void say_hello_using_thread_group(void)
{
  char stack_0[stack_bytes] __attribute__ ((aligned (STACK_ALIGN)));
  char stack_1[stack_bytes] __attribute__ ((aligned (STACK_ALIGN)));

  threadgroup_t group = thread_group_alloc(); 

  thread_group_add(group, say, "(TG1) Hello world!", stack_base(stack_0, stack_words)),
  thread_group_add(group, say, "(TG2) Hello world!", stack_base(stack_1, stack_words));

  thread_group_start(group);

  say("(TG Master) Hello world!");

  thread_group_wait_and_free(group);
}

static char stack_async[stack_bytes] __attribute__ ((aligned (STACK_ALIGN)));

void say_hello_asynchronously(void)
{
  run_async(say, "(Async) Hello world!", stack_base(stack_async, stack_words));
}

int main(void)
{
  say_hello_asynchronously();
  say_hello_using_xthreads();
  say_hello_using_thread_group();
}

