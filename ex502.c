#include <stdio.h>
#include <ctype.h>

#define SIZE 5

int getch(void) ;
void ungetch(int) ;

int main(void)
{
    int val, n, getfloat(float *);
    float array[SIZE];
    
    for (n = 0; n < SIZE && (val = getfloat(&array[n])) != EOF; n++)
        printf("array[%d] = %f\n", n, array[n]);

    return 0;
}

/* getfloat: get next float from input into *pn */
int getfloat(float *pn)
{
    int c, sign;
    
    while (isspace(c = getch())) // skip white space
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-' && c != '.') {
        ungetch(c); // it's not a number
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    if (c == '+' || c == '-') {
        while (isspace(c = getch()))
            ;
    }
    
    int factor = 1.0f;
    *pn = 0.0f;
    for ( ; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');
    if (c == '.') {
        c = getch();
    }
    for ( ; isdigit(c); c = getch()) {
        *pn = 10 * *pn + (c - '0');
        factor *= 10;
    }
    *pn /= factor;
    *pn *= sign;

    if (c != EOF)
        ungetch(c);

    return c;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

int getch(void) /* get a (possibly pushed back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if(bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
