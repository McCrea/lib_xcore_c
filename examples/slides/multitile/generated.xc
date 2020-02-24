// AUTO GENERATED FILE - DO NOT EDIT
#include <platform.h>

typedef chan channel_t;
typedef chanend chanend_t;
typedef port port_t;

port_t led_port = XS1_PORT_32A;
port_t button_port = XS1_PORT_1J;

extern "C"
{
void main_pipe(chanend_t, chanend_t);
void main_button(chanend_t, port_t);
void main_led(chanend_t, port_t);
}

main()
{
  channel_t led_chan;
  channel_t button_chan;
  par
  {
    main_pipe(led_chan, button_chan);
    main_button(button_chan, button_port);
    main_led(led_chan, led_port);
  }
}
