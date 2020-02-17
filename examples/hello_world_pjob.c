#include <xcore/parallel.h>
#include <stdio.h>

DECLARE_JOB(say_hello, (void));
void say_hello(void)
{
  puts("Hello world!");
}

int main(void)
{
  PAR_JOBS(
    PJOB(say_hello, ()),
    PJOB(say_hello, ()) );
}

