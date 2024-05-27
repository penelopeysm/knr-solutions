#include "my_getline.h"

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
