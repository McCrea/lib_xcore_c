#include <stdio.h>
#include <xcore/port.h>

int main(void)
{
  port_t button_port = XS1_PORT_1J;

  port_enable(button_port);

  unsigned port_value = port_peek(button_port);
  printf("Button port value: %u\n", port_value);

  port_set_invert(button_port);
  port_value = port_peek(button_port);
  printf("Button port value: %u\n", port_value);
  
  port_set_no_invert(button_port);
  port_value = port_peek(button_port);
  printf("Button port value: %u\n", port_value);

  port_disable(button_port);
}
