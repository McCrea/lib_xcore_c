#include <stdio.h>
#include <xcore/parallel.h>

DECLARE_JOB(print_int_sum, (int, int, int *));
void print_int_sum(int a, int b, int *sum)
{
  int result = a + b;
  printf("Int sum is: %d\n", result);
  *sum = result;
}

DECLARE_JOB(print_float_sum, (float, float, float *));
void print_float_sum(float a, float b, float *sum)
{
  float result = a + b;
  printf("Float sum is: %.1f\n", result);
  *sum = result;
}

int main(void)
{
  int i;
  float f;
  PAR_JOBS(
    PJOB(print_int_sum, (10, 5, &i)),
    PJOB(print_float_sum, (10.0, 5.0, &f))
  );
  printf("The results were: %d and %.1f\n", i, f);
}

