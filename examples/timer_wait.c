#include <stdio.h>
#include <xcore/hwtimer.h>

int main(void)
{
  hwtimer_t timer = hwtimer_alloc();

  unsigned time = hwtimer_get_time(timer);

  // Wait for the timer value to advance by 100000000
  unsigned wait_until = time + 100000000;
  printf("Current time: %u, waiting until %u\n", time, wait_until);
  time = hwtimer_wait_until(timer, wait_until);
  printf("Actually waited until %u\n", time);

  // Setting the trigger and then getting the time will delay until the trigger
  time = hwtimer_get_time(timer);
  wait_until = time + 100000000;
  hwtimer_set_trigger_time(timer, wait_until);
  time = hwtimer_get_time(timer);
  printf("Set trigger time to %u; hwtimer_get_time returned %u\n", wait_until, time);

  // If we wait again immediately then the call won't block 
  time = hwtimer_get_time(timer);
  printf("Time after waiting again with trigger unchanged %u\n", time);

  // Similarly, as the timer triggers on values between (trigger_value) and (trigger_value - 2^(bits_per_word-1)) 
  // If we wait 'longer' than 2^*bits_per_word-1) then the call won't block. (Note wraparound.)
  wait_until = hwtimer_get_time(timer) + (1u << 31) + 200000000;
  hwtimer_change_trigger_time(timer, wait_until);
  time = hwtimer_get_time(timer);
  printf("hwtimer_get_time returned %u; trigger time was %u\n", time, wait_until);

  hwtimer_free(timer);
}

