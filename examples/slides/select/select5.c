#include <stdio.h>
#include <xcore/hwtimer.h>
#include <xcore/select.h>

int main(void)
{
  hwtimer_t t = hwtimer_alloc();
  unsigned long now = hwtimer_get_time(t);

  int default_last = 0;
  hwtimer_set_trigger_time(t, now + 10000000);
  SELECT_RES_ORDERED(
    CASE_GUARD_THEN(t, default_last, timer_handler),
    DEFAULT_NGUARD_THEN(default_last, default_handler))
  {
  timer_handler:
    now = hwtimer_get_time(t);
    hwtimer_change_trigger_time(t, now + 10000000);
    printf("Timer handler at time: %lu\n", now);
    default_last = 0;
    continue;
  default_handler:
    puts("Nothing happened...");
    default_last = 1;
    continue;
  }

  hwtimer_free(t);
}

