#include <platform.h>
#include <xcore/port.h>

void port_user(port_t p, port_t p_out)
{
  port_set_invert(p);
  port_set_trigger_in_not_equal(p, port_peek(p));

  int a = port_in(p); //Now blocking
  port_out(p_out, a);
}

void port_user2(port_t p);

int main(void)
{
  port_t my_port = XS1_PORT_1J,
         my_output_port = XS1_PORT_32A;
  port_enable(my_port);
  port_enable(my_output_port);
  port_user(my_port, my_output_port);
  port_reset(my_port);
  port_user2(my_port); //!!! May not expect port in to block
  port_disable(my_port);
  port_disable(my_output_port);
}

