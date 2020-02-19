#include <platform.h>
#include <xcore/hwtimer.h>

int main(void)
{
  hwtimer_t t = hwtimer_alloc();

  unsigned v = hwtimer_get_time(t);
  hwtimer_set_trigger_time(t, v + 100000000);
  v = hwtimer_get_time(t); // Now blocking

  hwtimer_change_trigger_time(t, hwtimer_get_trigger_time(t) + 100000000);
  v = hwtimer_get_time(t); // Now blocking

  hwtimer_wait_until(t, v + 100000000);

  hwtimer_delay(t, 100000000);

  hwtimer_free(t);
}

