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

  threadgroup_t group = thread_group_alloc(); 

  thread_group_add(group, say_hello, NULL, stack_base(stack_0, 256)),
  thread_group_add(group, say_hello, NULL, stack_base(stack_1, 256));

  thread_group_start(group);

  say_hello(NULL);

  thread_group_wait_and_free(group);
}

