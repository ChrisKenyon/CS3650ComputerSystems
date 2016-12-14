#include<stdio.h>
int main(int argc, char* argv[])
{
    int i, numberOfWhite, numberOfOther;
    char x1[100] = "The 25 quick brown foxes jumped over the 27 lazy dogs 17 times.";
    char *x1ptr = x1;
    int ndigit[10];
    int *ndPtr = ndigit;
    for (i = 0; i<10; i++)
        *(ndPtr+i) = 0;
    i = numberOfWhite = numberOfOther = 0;
    while (*(x1ptr+i) != '\0'){
        if (*(x1ptr+i) >= '0' && *(x1ptr+i) <= '9')
            (*(ndPtr + *(x1ptr+i) -'0'))++;
        else if (*(x1ptr+i) == ' ' || *(x1ptr+i) == '\n' || *(x1ptr+i) == '\t')
            numberOfWhite++;
        else
            numberOfOther++;
        i++;
    }
    printf("digits =");
    for (i = 0; i<10; ++i)
        printf(" %d", *(ndPtr+i));
    printf(", white space = %d, other = %d\n", numberOfWhite, numberOfOther);
    return 0;
}
