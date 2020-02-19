#include <stdio.h>
#include <xcore/hwtimer.h>
#include <xcore/select.h>

int main(void)
{
  hwtimer_t t = hwtimer_alloc();
  unsigned long now = hwtimer_get_time(t);

  hwtimer_set_trigger_time(t, now + 10000000);
  SELECT_RES(
    CASE_THEN(t, timer_handler))
  {
  timer_handler:
    now = hwtimer_get_time(t);
    printf("Timer handler at time: %lu\n", now);
    break;
  }

  hwtimer_free(t);
}

