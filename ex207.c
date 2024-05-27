#include <stdio.h>
#include <stdlib.h> // for atoi()
#define MAXLINE 1000

int my_getline(char s[], int lim) ;
int invert(int deci, int p, int n) ;

int main(void)
{
    int p = 5;
    int n = 3;
    int deci;
    char s[MAXLINE];
    int len;

    while ((len = my_getline(s, MAXLINE)) != 0) {
        deci = atoi(s);
        int res = invert(deci, p, n);
        printf("invert(%d,%d,%d) = %d\n", deci, p, n, res);
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

int invert(int deci, int p, int n)
{
    int mask = (~0 << (p + 1 - n)) ^ (~0 << (p + 1)); // 00111000
    // alternatively, equivalently,
    // int mask = ~(~0 << n) << (p + 1 - n);
    int inverted_bits = (~deci) & mask; // everything not inverted = 0
    int retained_bits = deci & (~mask); 
    return (inverted_bits | retained_bits);
}
