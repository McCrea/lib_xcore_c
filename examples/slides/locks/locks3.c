#include <xcore/lock.h>

int main(void)
{
  lock_t l = lock_alloc();

  lock_acquire(l); // Blocks until available
  lock_acquire(l);

  lock_release(l);
  lock_release(l); // Error: ILLEGAL_RESOURCE 

  lock_free(l);
}


