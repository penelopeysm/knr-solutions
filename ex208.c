#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXLINE 1000

int my_getline(char s[], int lim) ;
int rightrot(int x, int n, int bits) ;

int main(void)
{
    int x;
    int n;
    char s[MAXLINE];
    char sn[MAXLINE];

    // need code to find how many bits an integer is
    int i = (~0);
    int bits = 0;
    while (i != 0) {
        i = (i << 1);
        ++bits;
    }

    while (my_getline(s, MAXLINE) != 0) {
        x = atoi(s);
        if (my_getline(sn, MAXLINE) != 0){
            n = atoi(sn);
            int res = rightrot(x,n,bits);
            printf("rightrot(%d,%d) = %d\n", x, n, res);
        }
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

int rightrot(x,n,bits)
{
    n = n % bits;
    int right_bits = x >> n;
    int wrong_bits = x << (bits - n);
    return (right_bits | wrong_bits);
}
