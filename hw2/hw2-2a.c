#include<stdio.h>
int main(int argc, char* argv[])
{
    int i, numberOfWhite, numberOfOther;
    char x1[100] = "The 25 quick brown foxes jumped over the 27 lazy dogs 17 times.";
    int ndigit[10];
    for (i = 0; i<10; i++)
        ndigit[i] = 0;
    i = numberOfWhite = numberOfOther = 0;
    while (x1[i] != '\0'){
        if (x1[i] >= '0' && x1[i] <= '9')
            ++ndigit[x1[i]-'0'];
        else if (x1[i] == ' ' || x1[i] == '\n' || x1[i] == '\t')
            numberOfWhite++;
        else
            numberOfOther++;
        i++;
    }
    printf("digits =");
    for (i = 0; i<10; ++i)
        printf(" %d", ndigit[i]);
    printf(", white space = %d, other = %d\n", numberOfWhite, numberOfOther);
    return 0;
}
