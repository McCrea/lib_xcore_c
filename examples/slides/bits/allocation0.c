#include <xcore/channel.h>
#include <xcore/lock.h>

void my_function(lock_t, channel_t);

int main(void)
{
  lock_t l = lock_alloc();
  channel_t c = chan_alloc();

  //Resources might be invalid!
  my_function(l, c);

  lock_free(l);
  chan_free(c);
}
