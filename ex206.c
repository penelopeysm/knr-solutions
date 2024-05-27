#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000

int my_getline(char s[], int lim) ;
int setbits(int x, int p, int n, int y) ;

int main(void)
{
    int x;
    char inp[MAXLINE];
    int p = 5;
    int n = 2;
    int y = 41; // 32 + 8 + 1 = 101001 in binary
    int len;
    while ((len = my_getline(inp, MAXLINE)) != 0) {
        int deci = atoi(inp);
        int result = setbits(deci, p, n, y);
        printf("setbits(%d,%d,%d,%d) is %d\n", deci, p, n, y, result);
    }
    return 0;
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

int setbits(int deci, int p, int n, int y)
{
    y = (y & ~(~0 << n)); // masks all but the right-hand n bits of y - see main text for explanation
    y = y << (p + 1 - n); // left-shifts so that the bits of interest start at position p
    int mask = ~(~(~0 << n) << (p + 1 - n));
    return ((deci & mask) | y) ;
    // the code could be further shortened to fit everything in one line:
    // return ((deci & ~(~(~0 << n) << (p + 1 - n))) | ((y & ~(~0 << n)) << (p + 1 - n)));
}
