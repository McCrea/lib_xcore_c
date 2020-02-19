#include <platform.h>
#include <xcore/port.h>

void port_user(port_t p)
{
}

int main(void)
{
  port_t my_port = XS1_PORT_1J;
  port_enable(my_port);
  port_user(my_port);
  port_disable(my_port);
}

