#include <stdio.h>

char *s = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 !\"#%&'()*+,-./:;<=>?[\\]^_{|}~\t\f\v\r\n"; /* shamelessly stolen from internet, I don't have the patience to do it myself */

int main(void)
{
    int c;
    int ccount;
    while ((c = getchar()) != EOF) {
        /* count characters since last newline */
        if (c != '\n') ccount++;
        else ccount = 0;
        if (ccount >= 80) { /* this will totally ruin your Python code but at least it's PEP8 compliant */
            putchar('\n');
            ccount = 0;
        }

        /* print the character */
        int printable = 0;
        char *p = s;
        while (*p++) { 
            if (c == *p) {
                printable = 1;
                break;
            }
        }

        if (printable) putchar(c);
        else {
            printf("%02X ", c);
            ccount += 3;
        }
    }
}
