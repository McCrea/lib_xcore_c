#include <stdio.h>
#include <xcore/lock.h>
#include <xcore/parallel.h>

DECLARE_JOB(count_to_four, (lock_t));
void count_to_four(const lock_t lock)
{
  lock_acquire(lock);

  puts("One");
  puts("Two");
  puts("Three");
  puts("Four");

  lock_release(lock);
}

int main(void)
{
  lock_t lock = lock_alloc();

  PAR_JOBS(
    PJOB(count_to_four, (lock)),
    PJOB(count_to_four, (lock)));

  lock_free(lock);
}

