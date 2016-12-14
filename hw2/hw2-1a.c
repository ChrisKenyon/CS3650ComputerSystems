#include<stdio.h>
/*
 * Define the delimeter states for whether we're at a new word
 */
#define DS_NEW 1
#define DS_START 0
int main(int argc, char* argv[])
{
    int i, numberOfLines, numberOfWords, numberOfChars;
    char x1[100] = "The quick brown fox jumped over the lazy dog.\n";
    i = numberOfLines = numberOfWords = numberOfChars = 0;
    int state = DS_NEW;
    while (x1[i] != '\0'){
        numberOfChars++;
        if (x1[i] == '\n')
            numberOfLines++;
        if (x1[i] == ' ' || x1[i] == '\n' || x1[i] == '\t')
            state = DS_NEW;
        else if (state == DS_NEW){
            state = DS_START;
            numberOfWords++;
        }
        i++;
    }
    printf("%d %d %d\n", numberOfLines, numberOfWords, numberOfChars);
    return 0;
}
