#include <stdio.h>

int print_int_sum(int a, int b)
{
  int result = a + b;
  printf("Int sum is: %d\n", result);
  return result;
}

float print_float_sum(float a, float b)
{
  float result = a + b;
  printf("Float sum is: %.1f\n", result);
  return result;
}

int main(void)
{
  int i;
  float f;
  par {
    i = print_int_sum(10, 5);
    f = print_float_sum(10.0, 5.0);
  }
  printf("The results were: %d and %.1f\n", i, f);
}

