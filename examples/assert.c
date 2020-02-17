#include <stdio.h>
#include <xcore/assert.h>
#include <xcore/hwtimer.h>

int main(void)
{
  hwtimer_t timer = hwtimer_alloc();

  unsigned time = hwtimer_get_time(timer);

//  xassert(1);
//  xassert(1);
//  xassert(0);
//  xassert(0);


  // Wait for the timer value to advance by 100000000
  unsigned wait_until = time + 100000000;

  xassert(wait_until);
  if (wait_until)
  {
    puts("I'm true!");
  }

  while(1)
  {
    time = hwtimer_get_time(timer);
    xassert(time < wait_until);
    xassert_not(time >= wait_until);
    puts("I'm still here...");
  }

  printf("Current time: %u, waiting until %u\n", time, wait_until);
  time = hwtimer_wait_until(timer, wait_until);
  printf("Actually waited until %u\n", time);

  time = hwtimer_get_time(timer);
  xassert_not_after(time + 100000000);
  xassert_not_after(time - (1u << 31));

  hwtimer_free(timer);
}

