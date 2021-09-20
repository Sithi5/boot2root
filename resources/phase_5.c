#include <string.h>
#include <stdio.h>

#define ASCI_START_CHAR 'a'
#define ASCI_END_CHAR 'z'

void charset(char *string, char *line)
{
    char set[16] = "isrveawhobpnutfg";
    int j;
    int i;
    for (i = 0; i < 26; i++)
    {
        j = (int)string[i] & 15;
        string[i] = set[j];
    }
}

int main(void)
{
    char input[26] = "abcdefghijklmnopqrstuvwxyz\0";
    char line[7] = "giants\0";

    charset(input, line);
    printf("input = %s\n", input);
    return 0;
}

// g = 15 == o
// i = 16 == p
// a = 5  == e
// n == 11 == k
// t == 14 == m
// s = 1   == q