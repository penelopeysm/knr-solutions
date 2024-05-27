#include <stdio.h>
#include <string.h>
#include <limits.h>
#define MAXLINE 1000

void itob (int n, char s[], int base) ;
void reverse (char s[]) ;

int main ()
{
    char s1[MAXLINE];
    int a = 159;
    itob(a, s1, 16);
    printf("itob(%d) is %s\n", a, s1);
    printf("INT_MIN is %d\n", INT_MIN);
    char s2[MAXLINE];
    itob(INT_MIN, s2, 16);
    printf("itob(INT_MIN) is %s\n", s2);
    return 0;
}

void itob (int n, char s[], int base)
{
    int i, sign;
    sign = n; // still need a temp variable to determine the sign
    // don't reverse the sign of n, but rather
    // reverse the sign of each modulus 
    /*
    if ( (sign = n) < 0 )
        n = -n;
    */
    i = 0;
    do {
        if (n % base >= 0 && n % base < 10)
            s[i++] = (n % base + '0');
        else if (n % base >= 10)
            s[i++] = (n % base - 10 + 'A');
        else if (n % base > -10 && n % base < 0)
            s[i++] = (-(n % base) + '0');
        else if (n % base <= -10)
            s[i++] = (-(n % base) - 10 + 'A');
    } while ( (n /= base) != 0); // change > to != because n can be negative now
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void reverse (char s[])
{
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; ++i, --j) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
