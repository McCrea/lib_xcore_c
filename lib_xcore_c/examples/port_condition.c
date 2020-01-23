#include <stdio.h>
#include <xcore/port.h>

int main(void)
{
  port_t button_port = XS1_PORT_1J;

  port_enable(button_port);

  unsigned port_value = port_in(button_port);
  printf("Button port value: %u\n", port_value);

  port_value = port_in_when_pinsneq(button_port, PORT_UNBUFFERED, port_value);
  printf("Button port value: %u\n", port_value);

  port_value = port_in_when_pinseq(button_port, PORT_UNBUFFERED, port_value);
  printf("Button port value: %u\n", port_value);

  //We can do the same without completely setting up and tearing down the trigger each time:
 

  port_set_trigger_in_equal(button_port, !port_value);
  port_enable_trigger(button_port);
  port_value = port_in(button_port);
  printf("Button port value: %u\n", port_value);

  // Can change just the value
  port_set_trigger_value(button_port, !port_value);
  port_value = port_in(button_port);
  printf("Button port value: %u\n", port_value);

  port_disable_trigger(button_port);

  port_disable(button_port);
}
