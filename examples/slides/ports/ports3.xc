#include <platform.h>

port my_port = XS1_PORT_1J,
     my_output_port = XS1_PORT_32A;

void port_user(port p, port p_out)
{
  "???";
}

int main(void)
{
  port_user(my_port, my_output_port);
}

