#include <xcore/thread.h>
#include <stdio.h>

void say_hello(void *unused)
{
  puts("Hello world!");
}

int main(void)
{
  char stack_0[4*256] __attribute__ ((aligned (4)));
  char stack_1[4*256] __attribute__ ((aligned (4)));

  xthread_t thread_0 = xthread_alloc_and_start(say_hello, NULL, stack_base(stack_0, 256)),
            thread_1 = xthread_alloc_and_start(say_hello, NULL, stack_base(stack_1, 256));

  say_hello(NULL);

  xthread_wait_and_free(thread_0);
  xthread_wait_and_free(thread_1);
}

