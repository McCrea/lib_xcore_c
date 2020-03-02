#include <stdio.h>
#include <xcore/hwtimer.h>
#include <xcore/select.h>

int main(void)
{
  hwtimer_t t = hwtimer_alloc();
  unsigned long now = hwtimer_get_time(t);

  hwtimer_set_trigger_time(t, now + 10000000);
  SELECT_RES(
    CASE_THEN(t, timer_handler),
    DEFAULT_THEN(default_handler))
  {
  timer_handler:
    now = hwtimer_get_time(t);
    hwtimer_change_trigger_time(t, now + 10000000);
    printf("Timer handler at time: %lu\n", now);
    continue;
  default_handler:
    puts("Nothing happened...");
    continue;
  }

  hwtimer_free(t);
}
