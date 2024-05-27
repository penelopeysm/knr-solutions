#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 500

void getbasename(char *, char *);

int main(int argc, char **argv)
{
    char name[MAXWORD];
    /* parse the name of the executable */
    getbasename(name, *argv);
    argc--;

    /* complain if there are still any arguments */
    if (argc) {
        fprintf(stderr, "extra arguments ignored: ");
        while (argc--) {
            fprintf(stderr, "%s ", *(++argv));
        }
        fprintf(stderr, "\n");
    }

    /* printf("name is: %s\n", name); */

    int (*conv)(int); /* pointer to conversion function */
    if (strcmp(name, "lower") == 0) conv = tolower;
    else if (strcmp(name, "upper") == 0) conv = toupper;
    else {
        fprintf(stderr, "non-standard filename: %s\nplease use 'lower' or 'upper'\n", name);
        return 1;
    }

    int c;
    while ((c = getchar()) != EOF) putchar(conv(c));
    return 0;
}

void getbasename(char *s, char *t)
{
    int count = 0;
    while ((*s++ = *t++)) {
        count++;
        if (*(s-1) == '/') s -= count;
    }
}
