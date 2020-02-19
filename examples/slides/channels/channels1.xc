void sends_first(chanend c)
{
  c <: 1;
  c <: 5;
}

void receives_first(chanend c)
{
  int a;
  c :> a;
  c :> a;
}

int main(void)
{
  chan c;
  par {
    sends_first(c);
    receives_first(c);
  }
}

