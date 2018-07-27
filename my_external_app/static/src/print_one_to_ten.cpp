#include <stdio.h>

void print_one_to_ten()
{
  printf("print_one_to_ten() has been called.\n");
  for (unsigned int i = 1; i <= 10; i++)
  {
    printf("%d\n", i);
  }
}
