#include <string.h>
#include <stdio.h>

#define ASCI_START_CHAR 'a'
#define ASCI_END_CHAR 'z'

void rot_decode(char *string, int rot)
{
    int len = strlen(string);

    while (len-- > 0)
    {
        string[len] = string[len] - rot;
        if (string[len] < ASCI_START_CHAR)
        {
            string[len] = string[len] + ASCI_END_CHAR - ASCI_START_CHAR;
        }
    }
}

int main(void)
{
    char input[7] = "giants\0";
    char input2[7] = "giants\0";

    printf("input string = %s\n", input);
    rot_decode(input, 17);
    printf("rot 17 = %s\n", input);
    return 0;
}