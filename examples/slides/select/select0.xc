#include <stdio.h>
#include <platform.h>

int main(void)
{
  [[hwtimer]]
  timer t;
  unsigned long now;
  t :> now;

  select
  {
  case t when timerafter(now + 10000000) :> now:
    printf("Timer handler at time: %lu\n", now);
    break;
  }
}

