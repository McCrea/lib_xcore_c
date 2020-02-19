#include <xcore/channel.h>
#include <xcore/parallel.h>

DECLARE_JOB(sends_first, (chanend_t));
void sends_first(chanend_t c)
{
  chan_out_word(c, 1);
  chan_out_byte(c, 5);
}

DECLARE_JOB(receives_first, (chanend_t));
void receives_first(chanend_t c)
{
  int a = chan_in_word(c);
  a = chan_in_byte(c);
}

int main(void)
{
  channel_t c = chan_alloc();
  PAR_JOBS(
    PJOB(sends_first, (c.end_a)),
    PJOB(receives_first, (c.end_b))
  );
  chan_free(c);
}

