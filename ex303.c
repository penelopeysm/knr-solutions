#include <stdio.h>
#define MAXLINE 1000

int my_getline(char s[], int lim) ;
void expand(char s1[], char s2[]) ;

int main(void)
{
    int len;
    char s[MAXLINE];
    char expanded[MAXLINE];

    while ( (len = my_getline(s, MAXLINE)) != 0) {
        expand(s, expanded);
        printf("%s", expanded);
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

void expand(char s1[], char s2[])
{
    int i = 0;
    int j = 0;
    while (s1[i] != '\0') {
        if ((s1[i] == '-') && (i > 0) && (s1[i+1] != '\n') && (s1[i+1] != '\0') && (s1[i+1] > s1[i-1])) {
            for (char c = (s1[i-1] + 1); c != s1[i+1]; c++) {
                s2[j++] = c;
            }
        }
        else {
            s2[j++] = s1[i];
        }
        ++i;
    }
    s2[j] = '\0';
}
