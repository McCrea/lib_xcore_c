#include <stdio.h>
#include <platform.h>

int main(void)
{
  [[hwtimer]]
  timer t;
  unsigned long now;
  t :> now;

  int default_last = 0;
  while (1)
  {
    [[ordered]]
    select
    {
    case default_last => t when timerafter(now + 10000000) :> now:
      printf("Timer handler at time: %lu\n", now);
      default_last = 0;
      break;
    !default_last => default:
      puts("Nothing happened...");
      default_last = 1;
      break;
    }
  }
}

