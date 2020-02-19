#include <stdio.h>

void print_int_sum(int a, int b)
{
  int result = a + b;
  printf("Int sum is: %d\n", result);
}

void print_float_sum(float a, float b)
{
  float result = a + b;
  printf("Float sum is: %.1f\n", result);
}

int main(void)
{
  par {
    print_int_sum(10, 5);
    print_float_sum(10.0, 5.0);
  }
}

