#include <platform.h>

port my_port = XS1_PORT_1J;

void port_user(port p)
{
  int a;
  p :> a;
}

int main(void)
{
  port_user(my_port);
}

