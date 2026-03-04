#include <stdio.h>
#include <ctype.h>

#define NUM 1
#define e_no_opt -1

int get_option(int type, const char *msg)
{
    char input[64];
    int index = 0;

    /* TEMP: simulate input */
    snprintf(input, sizeof(input), "abc42");

  if (fgets(input, sizeof(input), stdin) == NULL)
  {
    return e_no_opt;
  }

    if (type == NUM)
    {
        while (input[index] != '\0')
        {
            if (isdigit((unsigned char)input[index]))
            {
                return input[index] - '0';
            }
            index++;
        }
        return e_no_opt;
    }
    return e_no_opt;
}

int main(void)
{
    printf("%d\n", get_option(NUM, NULL));
}
