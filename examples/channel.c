#include <stdio.h>
#include <xcore/channel.h>
#include <xcore/parallel.h>


DECLARE_JOB(sends_first, (chanend_t));
void sends_first(const chanend_t c)
{
  int a = 5;
  printf("sends_first: Sending %d\n", a);
  chan_out_word(c, a);

  a = chan_in_word(c);
  printf("sends_first: Received %d\n", a);
}

DECLARE_JOB(receives_first, (chanend_t));
void receives_first(const chanend_t c)
{
  int a = chan_in_word(c);
  printf("receives_first: Received %d\n", a);

  a += 1;
  printf("receives_first: Sending %d\n", a);
  chan_out_word(c, a);
}

int main(void)
{
  channel_t channel = chan_alloc();

  PAR_JOBS(
    PJOB(sends_first, (channel.end_a)),
    PJOB(receives_first, (channel.end_b)));

  chan_free(channel);
}

