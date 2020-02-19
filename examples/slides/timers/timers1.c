#include <platform.h>
#include <xcore/hwtimer.h>

int main(void)
{
  hwtimer_t t = hwtimer_alloc();

  unsigned v = hwtimer_get_time(t);

  hwtimer_free(t);
}

