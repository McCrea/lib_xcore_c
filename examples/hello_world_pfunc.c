#include <xcore/parallel.h>
#include <stdio.h>

void say_hello(void *unused)
{
  puts("Hello world!");
}

int main(void)
{
  PAR_FUNCS(
    PFUNC(say_hello, NULL),
    PFUNC(say_hello, NULL));
}

