#include <stdio.h>

void func1(void)
{
  puts("Hello from func1!");
}

void func2(void)
{
  char cs_[64];
  puts("Hello from func2");
}

int main(void)
{
  void(*fp)(void) = func1;
  fp();
  fp = func2;
  fp();
}


