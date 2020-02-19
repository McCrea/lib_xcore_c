#include <xcore/lock.h>

int main(void)
{
  lock_t l = lock_alloc();
  lock_free(l);
}


