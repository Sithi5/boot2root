
// input 6 numbers
// Input number < 7
// Input number all different

void phase_6(char *input)

{
    int *piVar1;
    int j;
    int i;
    int tab_to_sort[6] = {253, 725, 301, 997, 212, 432};
    int sorted_tab[6];
    int tab[6];

    read_six_numbers(input, tab);
    i = 0;
    while (i < 6)
    {
        // Input number < 6
        if (tab[i] > 6)
        {
            explode_bomb();
        }
        j = i + 1;
        if (j < 6)
        {
            // Input number all different
            while (j < 6)
            {
                if (tab[i] == tab[j])
                {
                    explode_bomb();
                }
                j = j + 1;
            }
        }
        i = i + 1;
    }

    // ....
    // sorting tabs here and putting it to sorted_tab
    // ....

    i = 0;
    while (i < 5)
    {
        if (sorted_tab[i] < sorted_tab[i + 1])
        {
            explode_bomb();
        }
        i = i + 1;
    }
    return;
}