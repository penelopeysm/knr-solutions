#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0; /* size_t is an unsigned int */
    ssize_t nread; /* ssize_t ranges from -1 to INT_MAX - an unsigned int which can handle exit codes */

    int n1;
    int n2;
    char o;

    while (printf(">> ") && (nread = getline(&line, &len, stdin) > 0)) {
        if (sscanf(line, "%d %c %d", &n1, &o, &n2) == 3) {
            if (o == '+') printf("%d\n", n1 + n2);
            else if (o == '-') printf("%d\n", n1 - n2);
            else if ((o == '*') || (o == 'x')) printf("%d\n", n1 * n2);
            else if (o == '/') printf("%d\n", n1 / n2);
            else printf("unrecognised operand '%c'\n", o);
        }
        else printf("please provide input in the format of NUMBER OPERAND NUMBER\n");
    }

    return 0;
}
