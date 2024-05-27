#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000

int my_getline(char s[], int lim) ;
int bitcount(unsigned int x) ;

int main(void)
{
    int len;
    unsigned int deci;
    int result;
    char s[MAXLINE];
    while ( (len = my_getline(s, MAXLINE) != 0 )){
        deci = atoi(s);
        result = bitcount(deci);
        printf("bitcount(%d) = %d\n", deci, result);
    }
}

int my_getline(char s[], int lim)
{
    int c, i;
    for (i = 0; ((c = getchar()) != EOF && c != '\n'); ++i) {
        if (i < lim - 1) {
            s[i] = c;
        }
    }
    if ((c == '\n') && (i < lim - 1)) {
        s[i] = c;
        ++i;
    }
    s[i <= lim - 1 ? i : lim - 1] = '\0';
    return i;
}

int bitcount(unsigned x)
{
    // WLOG assume that the number in binary is xxxxx10000 (n 0's after final 1)
    // xxxxx10000 - 1 = xxxxx01111 (n 1's after a 0)
    // xxxxx10000 & xxxxx01111 = xxxxx00000
    int b = 0;
    while (x != 0) {
        x &= (x - 1);
        b++;
    }
    return b;
}

/*
int bitcount(unsigned x)
{
    int b;
    for (b = 0; x != 0; x >>= 1){
        if (x & 01){
            b++;
        }
    }
    return b;
}
*/

