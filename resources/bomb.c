#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void explode_bomb(void)

{
    printf("\nBOOM!!!\n");
    printf("The bomb has blown up.\n");
    /* WARNING: Subroutine does not return */
    exit(8);
}

void sig_handler(void)
{
    printf("So you think you can stop the bomb with ctrl-c, do you?\n");
    sleep(3);
    printf("Well...");
    fflush(stdout);
    sleep(1);
    printf("OK. :-)\n");
    /* WARNING: Subroutine does not return */
    exit(0x10);
}

void initialize_bomb(void)

{
    signal(2, sig_handler);
    return;
}

void phase_5(int input)

{
    // Input of 6 char

    int len;
    char compare_string[6];

    len = strlen(input);
    if (len != 6)
    {
        explode_bomb();
    }

    len = 0;
    while (len < 6)
    {
        compare_string[len] = input[len] & 15;
        len = len + 1;
    }
    len = strcmp(compare_string, "giants");
    if (len != 0)
    {
        explode_bomb();
    }
    return;
}

int func4(int input)

{
    int first_number;
    int second_number;

    if (input < 2)
    {
        second_number = 1;
    }
    else
    {
        first_number = func4(input - 1);
        second_number = func4(input - 2);
        second_number = second_number + first_number;
    }
    return second_number;
}

void phase_4(char *input)
{
    int ret;
    int input_number;

    // input = 9

    ret = sscanf(input, "%d", &input_number);
    if ((ret != 1) || (input_number < 1))
    {
        explode_bomb();
    }
    ret = func4(input_number);
    if (ret != 55)
    {
        explode_bomb();
    }
    return;
}

void phase_3(char *input)
{
    int ret;
    char compare_char;
    uint first_number;
    char first_char;
    int second_number;

    // input = 1 b 214

    // input = 2 b 755

    // input = 7 b 524

    ret = sscanf(input, "%d %c %d", &first_number, &first_char, &second_number);
    if (ret < 3)
    {
        explode_bomb();
    }
    switch (first_number)
    {
    case 0:
        compare_char = 'q';
        if (second_number != 777)
        {
            explode_bomb();
        }
        break;
    case 1:
        compare_char = 'b';
        if (second_number != 214)
        {
            explode_bomb();
        }
        break;
    case 2:
        compare_char = 'b';
        if (second_number != 755)
        {
            explode_bomb();
        }
        break;
    case 3:
        compare_char = 'k';
        if (second_number != 251)
        {
            explode_bomb();
        }
        break;
    case 4:
        compare_char = 'o';
        if (second_number != 160)
        {
            explode_bomb();
        }
        break;
    case 5:
        compare_char = 't';
        if (second_number != 458)
        {
            explode_bomb();
        }
        break;
    case 6:
        compare_char = 'v';
        if (second_number != 780)
        {
            explode_bomb();
        }
        break;
    case 7:
        compare_char = 'b';
        if (second_number != 524)
        {
            explode_bomb();
        }
        break;
    default:
        compare_char = 'x';
        explode_bomb();
    }
    if (compare_char != first_char)
    {
        explode_bomb();
    }
    return;
}

void read_six_numbers(char *input, int *param_2)

{
    int ret;

    ret = sscanf(input, "%d %d %d %d %d %d", param_2, param_2 + 4, param_2 + 8, param_2 + 0xc,
                 param_2 + 0x10, param_2 + 0x14);
    if (ret < 6)
    {
        explode_bomb();
    }
    return;
}

void phase_2(char *input)
// 1 = 1
// 2 = (2 * 1) = 2
// 3 = (3 * 2) = 6
// 4 = (4 * 6) = 24
// 5 = (5 * 24) = 120
// 6 = (6 * 120) = 720
// soluce = 1 2 6 24 120 720
{
    int i;
    int six_input_numbers[6];

    read_six_numbers(input, six_input_numbers);

    i = 1;
    if (six_input_numbers[0] != 1)
    {
        explode_bomb();
    }
    while (i < 6)
    {
        if (six_input_numbers[i] != (i + 1) * six_input_numbers[i - 1])
        {
            explode_bomb();
        }
        i++;
    }
    return;
}

void phase_1(char *input)

{
    int ret;

    ret = strcmp(input, "Public speaking is very easy.");
    if (ret != 0)
    {
        explode_bomb();
    }
    return;
}

// void secret_phase(void)
// {
//     char *uVar1;
//     int iVar2;

//     uVar1 = read_line();
//     iVar2 = __strtol_internal(uVar1, 0, 10, 0);
//     if (1000 < iVar2 - 1U)
//     {
//         explode_bomb();
//     }
//     iVar2 = fun7(n1, iVar2);
//     if (iVar2 != 7)
//     {
//         explode_bomb();
//     }
//     printf("Wow! You\'ve defused the secret stage!\n");
//     phase_defused();
//     return;
// }

void phase_defused(int ac, char **av)

{
    int ret;
    int *local_58[4];
    char *local_54[80];

    if (ac == 6)
    {
        ret = sscanf(av + 0xf0, "%d %s", local_58, local_54);
        if (ret == 2)
        {
            ret = strcmp(local_54, "austinpowers");
            if (ret == 0)
            {
                printf("Curses, you\'ve found the secret phase!\n");
                printf("But finding it and solving it are quite different...\n");
                // secret_phase();
            }
        }
        printf("Congratulations! You\'ve defused the bomb!\n");
    }
    return;
}

char *read_line(char *in)
{
    char *cptr;

    if (cptr = fgets(in, 9999, stdin))
    {
        while (*cptr == ' ' || *cptr == '\t')
        {
            cptr++;
        }
        return cptr;
    }
    else
    {
        return 0;
    }
}

int main(int ac, char **av)

{
    char *input;
    FILE *file;

    if (ac == 1)
    {
        file = stdin;
    }
    else
    {
        if (ac != 2)
        {
            printf("Usage: %s [<input_file>]\n", *av);
            /* WARNING: Subroutine does not return */
            exit(8);
        }
        file = fopen((char *)av[1], "r");
        if (!file)
        {
            printf("%s: Error: Couldn\'t open %s\n", *av, av[1]);
            /* WARNING: Subroutine does not return */
            exit(8);
        }
    }
    initialize_bomb();
    printf("Welcome this is my little bomb !!!! You have 6 stages with\n");
    printf("only one life good luck !! Have a nice day!\n");
    input = read_line(input);
    phase_1(input);
    phase_defused(ac, av);
    printf("Phase 1 defused. How about the next one?\n");
    input = read_line(input);
    phase_2(input);
    phase_defused(ac, av);
    printf("That\'s number 2.  Keep going!\n");
    // input = read_line();
    // phase_3(input);
    // phase_defused(ac);
    // printf("Halfway there!\n");
    // input = read_line();
    // phase_4(input);
    // phase_defused(ac);
    // printf("So you got that one.  Try this one.\n");
    // input = read_line();
    // phase_5(input);
    // phase_defused(ac);
    // printf("Good work!  On to the next...\n");
    // input = read_line();
    // phase_6(input);
    // phase_defused(ac);
    return 0;
}

int strings_not_equal(char *str_1, char *str_2)

{
    char current_char;
    int len_str_1;
    int len_str_2;

    len_str_1 = strlen(str_1);
    len_str_2 = strlen(str_2);
    if (len_str_1 == len_str_2)
    {
        current_char = *str_1;
        while (current_char != '\0')
        {
            if (*str_1 != *str_2)
            {
                return 1;
            }
            str_1 = str_1 + 1;
            str_2 = str_2 + 1;
            current_char = *str_1;
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

string[(int)(input[i] & 15)]
