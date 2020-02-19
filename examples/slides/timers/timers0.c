#include <platform.h>
#include <xcore/hwtimer.h>

int main(void)
{
  hwtimer_t t = hwtimer_alloc();
  hwtimer_free(t);
}

