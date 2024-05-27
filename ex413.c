#include <stdio.h>
#include <string.h>

void reverse (char s[]) ;

int main (void)
{
    char s[] = "Hello World!";
    printf("%s\n", s);
    reverse(s);
    printf("%s\n", s);

    char t[] = "Doc, note, I dissent. A fast never prevents a fatness. I diet on cod.";
    printf("%s\n", t);
    reverse(t);
    printf("%s\n", t);

    return 0;
}

void reverse (char s[])
{
    static int i;
    static int recursive = 0;
    if (!recursive) {
        i = 0;
    }
    int len = strlen(s);
    if (len - 1 <= 2 * i) return ; // terminates if there's no need to reverse

    char temp = s[i];
    s[i] = s[len - 1 - i];
    s[len - 1 - i] = temp;
    i++;
    recursive = 1;
    reverse(s);

    recursive = 0;
}
