#include<stdio.h>
int main(int argc, char* argv[])
{
    int i, numberOfWhite, numberOfOther;
    char x1[100] = "The 25 quick brown foxes jumped over the 27 lazy dogs 17 times.";
    char *x1ptr = x1;
    int ndigit[10];
    int *ndPtr = ndigit;
    for (i = 0; i<10; i++)
        *(ndPtr++) = 0;
    ndPtr = ndigit;
    numberOfWhite = numberOfOther = 0;
    while (*x1ptr != '\0'){
        if (*x1ptr >= '0' && *x1ptr <= '9')
            (*(ndPtr + *x1ptr -'0'))++;
        else if (*x1ptr == ' ' || *x1ptr == '\n' || *x1ptr == '\t')
            numberOfWhite++;
        else
            numberOfOther++;
        x1ptr++;
    }
    printf("digits =");
    for (i = 0; i<10; ++i)
        printf(" %d", *(ndPtr++));
    printf(", white space = %d, other = %d\n", numberOfWhite, numberOfOther);
    return 0;
}
