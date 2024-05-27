#include <stdio.h>
#include <ctype.h>

#define SIZE 1000

int main(void)
{
    int n, array[SIZE], getint(int *), val;
    for (n = 0; n < SIZE && (val = getint(&array[n])) != EOF; n++) {
        printf("array[%d] = %d \t value returned: %d \n", n, array[n], val);
    }
    return 0;
}

int getch(void) ;
void ungetch(int) ;

// The exact instructions seem to be rather ambiguous...
// If you just push the sign back, you get stuck in a loop (which terminates only after main() iterates through array without doing anything to it)
// But if you want to skip the whitespace, then there's no need for ungetch(); just add another loop to skip over whitespace after a + or a -.
// The latter is the approach I've taken here, but there are presumably equally valid approaches.
/* getint: get next integer from input into *pn */
int getint(int *pn)
{
    int c, sign;
    
    while (isspace(c = getch())) // skip white space
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c); // it's not a number
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    if (c == '+' || c == '-') {
        while (isspace(c = getch()))
            ;
    }
    
    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');
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
