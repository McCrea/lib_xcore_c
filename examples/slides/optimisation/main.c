void main_pipe(chanend_t led_chanend, chanend_t button_chanend)
{
  int button_up = 1;
  int button_event_count = 0;

  SELECT_RES_ORDERED(
    CASE_GUARD_THEN(button_chanend, button_event_count < 21, event_button_chanend),
    CASE_THEN(led_chanend, event_led_chanend),
    DEFAULT_NGUARD_THEN(button_up, default_label))
  {
  default_label:
    puts("Button is still down!");
    SELECT_RESET;

  event_button_chanend:
    {
      uint32_t tmp = chan_in_word(button_chanend);
      chan_out_word(led_chanend, tmp);
      button_up = tmp;
    }
    button_event_count += 1;
    continue;

  event_led_chanend:
    {
      uint32_t tmp = chan_in_word(led_chanend);
      chan_out_word(button_chanend, tmp);
    }
    continue;
  }
}
