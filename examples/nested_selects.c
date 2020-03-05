#include <stdio.h>
#include <xcore/hwtimer.h>
#include <xcore/port.h>
#include <xcore/parallel.h>
#include <xcore/select.h>
#include <xcore/channel.h>

DECLARE_JOB(sender, (chanend_t, int));
void sender(chanend_t c, int id)
{
  hwtimer_t t = hwtimer_alloc();

  for (int i = 0; i < 6; i += 1)
  {
    do {
      chan_out_word(c, id);
    } while (!chan_in_word(c));
    hwtimer_delay(t, 100000000);
  }

  hwtimer_free(t);
}

void nested_select(chanend_t end0, chanend_t end1)
{
  int c0 = 0, c1 = 0;
  SELECT_RES_ORDERED(
    CASE_THEN(end1, end1_handler),
    CASE_THEN(end0, end0_handler),
    DEFAULT_GUARD_THEN(c0 || c1, default_handler))
  {
  end0_handler:
    c0 += 1;
    (void)chan_in_word(end0);
    chan_out_word(end0, c0 > 10);
    puts("end0");
    continue;

  end1_handler:
    (void)chan_in_word(end1);
    c1 += 1;
    chan_out_word(end1, c1 > 10);
    puts("end1");
    SELECT_CONTINUE_NO_RESET;

  default_handler: break;
  }
}

DECLARE_JOB(selector, (chanend_t, chanend_t));
void selector(chanend_t end0, chanend_t end1)
{
  port_t button0 = XS1_PORT_1J,
	 button1 = XS1_PORT_1K;

  port_enable(button0);
  port_enable(button1);

  int b0_val = port_peek(button0),
      b1_val = port_peek(button1);

  int b0_counter = 3,
      b1_counter = 3;

  port_set_trigger_in_not_equal(button0, b0_val);
  port_set_trigger_in_not_equal(button1, b1_val);

  SELECT_RES(
    CASE_GUARD_THEN(button0, b0_counter, b0_handler),
    CASE_NGUARD_THEN(button1, b1_counter == 0, b1_handler),
    DEFAULT_GUARD_THEN(!b0_counter && ! b1_counter, stop))
  {
  b0_handler:
    {
      b0_val = port_in(button0);
      b0_counter -= 1;
      int c0 = 0, c1 = 0;
      SELECT_RES_ORDERED(
        CASE_THEN(end0, end0_handler),
        CASE_THEN(end1, end1_handler),
        DEFAULT_GUARD_THEN(c0 || c1, default_handler))
      {
      end0_handler:
        c0 += 1;
        (void)chan_in_word(end0);
        chan_out_word(end0, c0 > 10);
        puts("end0");
        continue;

      end1_handler:
        (void)chan_in_word(end1);
        c1 += 1;
        chan_out_word(end1, c1 > 10);
        puts("end1");
        SELECT_CONTINUE_NO_RESET;

      default_handler: break;
      }
    }
    continue;

  b1_handler:
    b1_counter -= 1;
    b1_val = port_in(button1);
    nested_select(end0, end1);
    SELECT_CONTINUE_RESET;

  stop: break;
  }
}

int main(void)
{

  channel_t chan0 = chan_alloc(), chan1 = chan_alloc();

  PAR_JOBS(
    PJOB(sender, (chan0.end_a, 0)),
    PJOB(sender, (chan1.end_a, 1)),
    PJOB(selector, (chan0.end_b, chan1.end_b)));

}


