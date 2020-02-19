#include <platform.h>

int main()
{
  [[hwtimer]]
  timer t;

  unsigned v;
  t :> v;
}

