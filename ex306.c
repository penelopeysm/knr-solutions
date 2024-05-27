#include <stdio.h>
#include <string.h>
#include <limits.h>
#define MAXLINE 1000

void itoa (int n, char s[], int pad) ;
void reverse (char s[]) ;
void pad (char s1[], int len, char s2[]) ;

int main ()
{
    char s1[MAXLINE];
    int a = 4200;
    itoa(a, s1, 20);
    printf("itoa(%d, 20) is %s\n", a, s1);
    printf("INT_MIN is %d\n", INT_MIN);
    char s2[MAXLINE];
    itoa(INT_MIN, s2, 20);
    printf("itoa(INT_MIN, 20) is %s\n", s2);
    char s3[MAXLINE];
    itoa(INT_MIN, s3, 8);
    printf("itoa(INT_MIN, 8) is %s\n", s3);
    return 0;
}

void itoa (int n, char s[], int padding)
{
    char s_temp[MAXLINE];
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
        // s_temp[i++] = n % 10 + '0';
        // can use abs(x) function here, needs stdlib.h
        // this is the same thing
        s_temp[i++] = (n > 0) ? (n % 10 + '0') : (-(n % 10) + '0');
    } while ( (n /= 10) != 0); // change > to != because n can be negative now
    if (sign < 0)
        s_temp[i++] = '-';
    s_temp[i] = '\0';
    reverse(s_temp);

    pad(s_temp, padding, s);
    // this function is general and can be used elsewhere, but 
    // in this specific case it's easier to simply add spaces
    // to the END of the string before reversing it
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

void pad (char s1[], int len, char s2[]) 
{
    // pads s1 until it is len characters long and puts it in s2
    // if s1 is shorter than len characters, s2 is a truncated version
    int i = 0;
    int j = 0;
    if (strlen(s1) >= len) {
        while (i < len && s1[i] != '\0') {
            s2[j++] = s1[i++];
        }
    }
    else {
        while (j < len - strlen(s1)){
            s2[j++] = ' ';
        }
        while (s1[i] != '\0'){
            s2[j++] = s1[i++];
        }
    }
    s2[j] = '\0';
}
