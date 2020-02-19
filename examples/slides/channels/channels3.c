#include <xcore/channel.h>
#include <xcore/parallel.h>

DECLARE_JOB(sends_first, (chanend_t));
void sends_first(chanend_t c)
{
  chan_out_word(c, 1);
  chan_out_byte(c, 5);

  uint32_t words[5] = {1,2,3,4,5};
  chan_out_buf_word(c, words, 5);

  unsigned char bytes[4] = {1,2,3,4};
  chan_out_buf_byte(c, bytes, 4);
}

DECLARE_JOB(receives_first, (chanend_t));
void receives_first(chanend_t c)
{
  int a = chan_in_word(c);
  a = chan_in_byte(c);

  uint32_t words[5];
  chan_in_buf_word(c, words, 5);
  
  unsigned char bytes[4];
  chan_in_buf_byte(c, bytes, 4);
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

