#include <stdio.h>

__attribute__(( fptrgroup("my_functions") ))
void func1(void)
{
  char cs[64]; cs[0] = 0;
  puts("Hello from func1!");
}
__attribute__(( fptrgroup("my_typo") ))
void func2(void)
{
  char cs[256]; cs[0] = 0;
  puts("Hello from func2");
}

int main(void)
{
  __attribute__(( fptrgroup("my_functions", 1) )) void(*fp)(void) = func1;
  fp();
  fp = func2;
  fp();
}


