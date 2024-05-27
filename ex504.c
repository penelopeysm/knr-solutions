#include <stdio.h>
#include <string.h> // for strlen()

int strend(char *s, char *t) ;

int main(void)
{
    char s1[100] = "hello";
    char s2[100] = "yes hello";
    char s3[100] = "no helo";

    printf("%d\n", strend(s2, s1));
    printf("%d\n", strend(s3, s1));

    char s4[100] = "AAAAAAAAAAAAAAAAAAAABBB";
    char s5[100] = "BBB";
    printf("%d\n", strend(s4, s5));
    printf("%d\n", strend(s4, s1));

    return 0;
}

int strend(char *s, char *t)
{
    int i, slen, tlen;
    slen = strlen(s);
    tlen = strlen(t);
    if (slen < tlen) {
        return 0;
    }
    else {
        s += slen - tlen;
        for (i = tlen; *(s+i) == *(t+i) && i >= 0; i--)
            ;
    }
    if (i == -1) {
        return 1;
    }
    else
        return 0;
}
