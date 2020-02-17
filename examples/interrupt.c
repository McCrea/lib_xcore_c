#include <stdio.h>
#include <xcore/hwtimer.h>
#include <xcore/triggerable.h>
#include <xcore/port.h>
#include <xcore/interrupt.h>
#include <xcore/interrupt_wrappers.h>

static int interrupt_count = 0;

DEFINE_INTERRUPT_PERMITTED(interrupt_handlers, void, interruptable_task, void)
{
  hwtimer_t timer = hwtimer_alloc();

  interrupt_unmask_all();
  while (interrupt_count < 10)
  {
    puts("I'm still running.");
    hwtimer_delay(timer, 100000000);
  }
  interrupt_mask_all();

  hwtimer_free(timer);
}

DEFINE_INTERRUPT_CALLBACK(interrupt_handlers, interrupt_task, button)
{
  port_set_trigger_in_not_equal(*(port_t *)button, 1);
  interrupt_count += 1;
  printf("(%x) caused an interrupt\n", *(port_t *)button);
}

int main(void)
{
  port_t button1 = XS1_PORT_1J,
         button2 = XS1_PORT_1K;

  port_enable(button1);
  port_enable(button2);

  triggerable_setup_interrupt_callback(button1, &button1, INTERRUPT_CALLBACK(interrupt_task));
  triggerable_setup_interrupt_callback(button2, &button2, INTERRUPT_CALLBACK(interrupt_task));

  port_set_trigger_in_not_equal(button1, 1);
  port_set_trigger_in_not_equal(button2, 1);

  triggerable_enable_trigger(button1);
  triggerable_enable_trigger(button2);

  INTERRUPT_PERMITTED(interruptable_task)();

  port_disable(button1);
  port_disable(button2);
}

