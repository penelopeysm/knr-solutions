#include <stdio.h>

int strrindex(char s[], char t[]) ;

int main(void)
{
    char s1[] = "Hello World!";
    char t1[] = "World";
    char s2[] = "The quick brown fox jumped over the lazy dog.";
    char t2a[] = "o";
    char t2b[] = "FOX";

    int index1 = strrindex(s1, t1);
    int index2a = strrindex(s2, t2a);
    int index2b = strrindex(s2, t2b);

    printf("%d\n", index1);
    printf("%d\n", index2a);
    printf("%d\n", index2b);
    
    return 0;
}

int strrindex(char s[], char t[])
{
    int i, j, k;
    int match = -1;
    for (i = 0; s[i] != '\0'; i++) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++) {
            ;
        }
        if (k > 0 && t[k] == '\0') {
            match = i;
        }
    }
    return match;
}
