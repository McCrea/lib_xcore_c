void my_function0(void)
{
  char a[100];
  a[0] = 0;
}

void my_function1(void)
{
  volatile char a[200];
  a[0] = 0;
}

int main(void)
{
  volatile int s = 100;
  volatile void(*fp)(void) = 0;
  fp();
  char a[s];
  a[10] = 0;
}


