#include <stdio.h>
#include <xcore/parallel.h>

void print_string(void *str)
{
  puts((const char *)str);
}

struct float_sum_args {
  float a;
  float b;
  float result;
};

void float_sum(void *args_)
{
  struct float_sum_args *args = args_;
  args->result = args->a + args->b;
}

int main(void)
{
  struct float_sum_args fs_args = {5.0, 10.0, 0};
  PAR_FUNCS(
    PFUNC(print_string, "Hello world!"),
    PFUNC(float_sum, &fs_args)
  );
  printf("%.1f + %.1f = %.1f\n", fs_args.a, fs_args.b, fs_args.result);
}

