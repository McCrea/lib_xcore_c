#include <stdio.h>
#include <xcore/parallel.h>

DECLARE_JOB(print_int_sum, (int, int));
void print_int_sum(int a, int b)
{
  int result = a + b;
  printf("Int sum is: %d\n", result);
}

DECLARE_JOB(print_float_sum, (float, float));
void print_float_sum(float a, float b)
{
  float result = a + b;
  printf("Float sum is: %.1f\n", result);
}

int main(void)
{
  PAR_JOBS(
    PJOB(print_int_sum, (10, 5)),
    PJOB(print_float_sum, (10.0, 5.0))
  );
}

