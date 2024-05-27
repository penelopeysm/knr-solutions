#include <stdio.h>
#include <ctype.h>
#include "my_getline.h"
#define MAXLINE 1000

double atof(char s[]) ;

int main(void)
{
    int len;
    char s[MAXLINE];
    while ( (len = my_getline(s, MAXLINE)) != 0) {
        double a = atof(s);
        printf("%f\n", a);
        printf("%f * 2 = %f\n", a, a*2);
    }
    return 0;
}

double atof(char s[])
{
    double val, power;
    int i, sign;

    for (i = 0; isspace(s[i]); i++) ;
    
    sign = (s[i] == '-') ? -1 : 1;

    if (s[i] == '+' || s[i] == '-') {
        i++;
    }
    for (val = 0.0; isdigit(s[i]); i++){
        val = 10.0 * val + (s[i] - '0');
    }
    if (s[i] == '.') {
        i++;
    }
    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }

    // New code for exercise 4.2
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        int ex_magn;
        int ex_sign = 1;
        if (s[i] == '-'){
            ex_sign = -1;
        }
        if (s[i] == '-' || s[i] == '+') {
            i++;
        }
        for (ex_magn = 0; isdigit(s[i]); i++) {
            ex_magn = ex_magn * 10 + (s[i] - '0');
        }
        int exponent = ex_magn * ex_sign;
        if (exponent < 0) {
            for ( ; exponent != 0; exponent++) {
                power *= 10.0;
            }
        }
        else if (exponent > 0) {
            for ( ; exponent != 0; exponent--) {
                power /= 10.0;
            }
        }
    }

    return sign * val / power;
}
