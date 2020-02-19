#include <stdio.h>
#include <xcore/parallel.h>

DECLARE_JOB(sum_range, (int, int, long long *));
void sum_range(int begin, int end, long long *result)
{
  volatile long long res = 0;

  for (int c = begin; c < end; c += 1)
  {
    res += c;
//    printf("c: %d\n", c);
  }

  printf("Intermediate sum of (%d , %d] is %lld\n", begin, end, res);
  *result = res;
}

DECLARE_JOB(parallel_sum_range, (int, int, long long *));
void parallel_sum_range(int begin, int end, long long *result)
{
  int mid1 = ((long long)begin + end ) / 2,
      mid0 = ((long long)begin + mid1) / 2,
      mid2 = ((long long)mid1  + end ) / 2;

  long long res0, res1, res2, res3;
  PAR_JOBS(
    PJOB(sum_range, (begin, mid0, &res0)),
    PJOB(sum_range, (mid0,  mid1, &res1)),
    PJOB(sum_range, (mid1,  mid2, &res2)),
    PJOB(sum_range, (mid2,  end,  &res3)));

  *result = res0 + res1 + res2 + res3;
  printf("Sum of (%d , %d] is %lld\n", begin, end, *result);
}

static const int start = -199999999;
static const int mid = 0;
static const int end = 200000000;

int main(void)
{
  long long res0, res1;
  PAR_JOBS(
    PJOB(parallel_sum_range, (start, mid, &res0)),
    PJOB(parallel_sum_range, (mid, end, &res1)));

  printf("Sum of range (%d , %d] is: %lld\n", start, end, res0 + res1);

}


