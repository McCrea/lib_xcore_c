void sends_first(chanend c)
{
}

void receives_first(chanend c)
{
}

int main(void)
{
  chan c;
  par {
    sends_first(c);
    receives_first(c);
  }
}

