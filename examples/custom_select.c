#include <stdio.h>
#include <xcore/triggerable.h>
#include <xcore/port.h>

int main(void)
{
  port_t button1 = XS1_PORT_1J,
	 button2 = XS1_PORT_1K;

  port_enable(button1);
  port_enable(button2);

  TRIGGERABLE_SETUP_EVENT_VECTOR(button1, button1_label);
  TRIGGERABLE_SETUP_EVENT_VECTOR(button2, button2_label);

  port_set_trigger_in_not_equal(button1, port_in(button1));
  port_set_trigger_in_not_equal(button2, port_in(button2));

  triggerable_enable_trigger(button1);
  triggerable_enable_trigger(button2);

  while(1)
  {
    TRIGGERABLE_WAIT_EVENT(button1_label, button2_label);

    button1_label:
      port_set_trigger_in_not_equal(button1, port_in(button1));
      puts("Event on button 1");
      triggerable_disable_trigger(button1);
      triggerable_enable_trigger(button2);
      continue;

    button2_label:
      port_set_trigger_in_not_equal(button2, port_in(button2));
      puts("Event on button 2");
      triggerable_enable_trigger(button1);
      triggerable_disable_trigger(button2);
      continue;
  }
}


