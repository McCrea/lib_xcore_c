#include <stdio.h>
#include <xcore/hwtimer.h>
#include <xcore/select.h>

void my_function_which_selects()
{
  hwtimer_t t = hwtimer_alloc();
  SELECT_RES(CASE_THEN(t, timer_handler))
  {
  timer_handler:
    puts("Inner handler...");
    break;
  }
  hwtimer_free(t);
}

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
    my_function_which_selects();
    default_last = 0;
    SELECT_RESET;
  default_handler:
    puts("Nothing happened...");
    default_last = 1;
    continue;
  }

  hwtimer_free(t);
}

