#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Implemented the return value of printf, as well as the field widths for str and int. Not for floats. */

int minprintf(char *fmt, ...);

int main (void)
{
    char *s = "Hello World!";
    int a1 = printf("printf:    testing string: |%s|\n", s);
    int b1 = minprintf("minprintf: testing string: |%s|\n", s);
    printf("\n");
    int a2 = printf("printf:    testing string: |%15s|\n", s);
    int b2 = minprintf("minprintf: testing string: |%15s|\n", s);
    printf("\n");
    int a3 = printf("printf:    testing string: |%-15s|\n", s);
    int b3 = minprintf("minprintf: testing string: |%-15s|\n", s);
    printf("\n");

    int m = 5;
    int n = -2;
    int p = 10000;
    int q = 2000;
    int a4 = printf("printf:    testing ints: |%d| |%d| |%d| |%d|\n", m, n, p, q);
    int b4 = minprintf("minprintf: testing ints: |%d| |%d| |%d| |%d|\n", m, n, p, q);
    printf("\n");
    int a5 = printf("printf:    testing ints: |%3d| |%3d| |%3d| |%3d|\n", m, n, p, q);
    int b5 = minprintf("minprintf: testing ints: |%3d| |%3d| |%3d| |%3d|\n", m, n, p, q);
    printf("\n");
    int a6 = printf("printf:    testing ints: |%-3d| |%-3d| |%-3d| |%-3d|\n", m, n, p, q);
    int b6 = minprintf("minprintf: testing ints: |%-3d| |%-3d| |%-3d| |%-3d|\n", m, n, p, q);
    printf("\n");

    printf("printf returned   : %d %d %d %d %d %d\n", a1, a2, a3, a4, a5, a6);
    printf("minprintf returned: %d %d %d %d %d %d\n", b1, b2, b3, b4, b5, b6);

    return 0;
}

int minprintf(char *fmt, ...)
{
    va_list ap;
    char *p, *sval;
    char c;
    int ival;
    double dval;

    int count; /* the value to return */
    count = 0;

    char w[10];
    char *width = w;
    int lalign = 0; /* 1 for left-aligned */

    va_start(ap, fmt); /* makes ap point to the first unnamed argument */
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            count++;
            continue;
        }

        /* reach here if *p is % */
        c = *++p; /* get the next character */
        /* printf("-%c-", c); */

        lalign = 0;
        if (c == '-') { /* check for alignment */
            lalign = 1;
            c = *++p;
        }

        while (isdigit(c)) {
            *width++ = c;
            c = *++p;
        }
        *width = '\0';
        int widthn;
        widthn = atoi(w);
        width = w; /* reset the pointer */

        if (c == 'd') {
            ival = va_arg(ap, int);
            char ii[20]; /* longer than the largest number, in principle should check with INT_MAX */
            char *intasstr = ii;
            sprintf(intasstr, "%d", ival);
            if (*w) {
                int nspaces = widthn - strlen(intasstr);
                if (nspaces > 0) {
                    if (lalign == 1) { /* print int then spaces */
                        while (*intasstr) putchar(*intasstr++);
                        while (nspaces-- > 0) putchar(' ');
                    }
                    else if (lalign == 0) { /* print spaces then int */
                        while (nspaces-- > 0) putchar(' ');
                        while (*intasstr) putchar(*intasstr++);
                    }
                    count += widthn;
                }
                else { /* field width smaller than integer length, just print the int */
                    while (*intasstr) {
                        putchar(*intasstr++);
                        count++;
                    }
                }
            }
            else { /* no field width specified, just print the int */
                while (*intasstr) {
                    putchar(*intasstr++);
                    count++;
                }
            }
        }
        else if (c == 'f') {
            dval = va_arg(ap, double);
            printf("%f", dval);
        }
        else if (c == 's') {
            sval = va_arg(ap, char *);
            if ((*w) && (widthn > strlen(sval))) {
                int nspaces = widthn - strlen(sval);
                /* nothing to do if widthn <= strlen(sval) */
                if (lalign == 1) { /* print string then spaces */
                    while (*sval) putchar(*sval++);
                    while (nspaces-- > 0) putchar(' ');
                }
                else if (lalign == 0) { /* print spaces then string */
                    while (nspaces-- > 0) putchar(' ');
                    while (*sval) putchar(*sval++);
                }
                count += widthn;
            }
            else { /* just print the string */
                while (*sval) {
                    putchar(*sval++);
                    count++;
                }
            }
        }
        else {
            putchar('%');
            if (lalign == 1) putchar('-');
            if (*w) printf("%s", width); /* if width is not empty */
            putchar(*p);
            count += 2;
        }
    }
    va_end(ap);
    width[0] = '\0';

    return count;
}



