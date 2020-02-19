#include <stdio.h>

void func1(void)
{
  char cs[64]; cs[0] = 0;
  puts("Hello from func1!");
}

void func2(void)
{
  char cs[256]; cs[0] = 0;
  puts("Hello from func2");
}

int main(void)
{
  void(*fp)(void) = func1;
  fp();
  fp = func2;
  fp();
}


