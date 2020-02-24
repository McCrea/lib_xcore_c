#include <stdio.h>
#include <xcore/chanend.h>
#include <xcore/parallel.h>
#include <xcore/assert.h>

DECLARE_JOB(thread0, (chanend_t));
void thread0(chanend_t c)
{
  chanend_out_control_token(c, XS1_CT_END);
  chanend_check_control_token(c, XS1_CT_END);
  puts("0: Handshake done");
  chanend_out_byte(c, 'a');
  chanend_out_control_token(c, 12);
  chanend_out_control_token(c, 11);
  chanend_out_word(c, 0xdeadbeef);
  chanend_out_control_token(c, XS1_CT_END);
  chanend_check_control_token(c, XS1_CT_END);
  puts("0: Done!");
}

DECLARE_JOB(thread1, (chanend_t));
void thread1(chanend_t c)
{
  chanend_check_control_token(c, XS1_CT_END);
  chanend_out_control_token(c, XS1_CT_END);
  puts("1: Handshake done");
  unsigned t = chanend_test_control_token_next_byte(c);
  xassert_not(t);
  t = chanend_test_control_token_next_word(c);
  xassert(t == 2);
  char b = chanend_in_byte(c);
  printf("1: 0 sends char: %c\n", b);
  t = chanend_test_control_token_next_byte(c);
  xassert(t);
  b = chanend_in_control_token(c);
  printf("1: 0 sends control token: %d\n", (int)b);
  chanend_check_control_token(c, 11);
  uint32_t w = chanend_in_word(c);
  printf("1: 0 sends word: 0x%lx\n", w);
  chanend_check_control_token(c, XS1_CT_END);
  chanend_out_control_token(c, XS1_CT_END);
  t = chanend_test_dest_local(c);
  xassert(t);
  puts("1: Done!");
}

int main(void)
{
  chanend_t c0 = chanend_alloc(),
            c1 = chanend_alloc();

  chanend_set_dest(c0, c1);
  chanend_set_dest(chanend_get_dest(c0), c0);

  PAR_JOBS(
    PJOB(thread0, (c0)),
    PJOB(thread1, (c1)));

  chanend_free(c0);
  chanend_free(c1);
}

