#include<stdio.h>
/*
 * Define the delimeter states for whether we're at a new word
 */
#define DS_NEW 1
#define DS_START 0
int main(int argc, char* argv[])
{
    int numberOfLines, numberOfWords, numberOfChars;
    char x1[100] = "The quick brown fox jumped over the lazy dog.\n";
    char *x1ptr = x1;
    numberOfLines = numberOfWords = numberOfChars = 0;
    int state = DS_NEW;
    while (*x1ptr != '\0'){
        if (*x1ptr == '\n')
            numberOfLines++;

        if (*x1ptr == ' ' || *x1ptr== '\n' || *x1ptr == '\t')
            state = DS_NEW;
        else if (state == DS_NEW){
            state = DS_START;
            numberOfWords++;
        }
        *x1ptr++;
    }
    numberOfChars = x1ptr-x1;
    printf("%d %d %d\n", numberOfLines, numberOfWords, numberOfChars);
    return 0;
}
