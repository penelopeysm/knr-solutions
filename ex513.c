#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCHARS 10000
#define DEFAULT_N 10

#define NUMBER 1
#define MINUS_NUMBER 2
#define PLUS_NUMBER 3

/* to read a file from stdin: cat file | ./ex513, or ./ex513 < file */

/* This code gives similar output to the Unix program tail, but appears to be significantly slower:
 *     ./ex513 < longfile.txt
 * takes ca 1.5 seconds, but
 *     tail < longfile.txt
 * takes 4 ms. This is because tail doesn't actually read the entire file; it skips to the end of the
 * file, then rewinds to find the last n lines. See https://unix.stackexchange.com/q/102905. */

void printlines (char (*lines)[MAXCHARS], int n, int nlines) ; 
int my_getline(char *s, int lim) ; /* Note that this is NOT the same function as in my_getline.c, see below for explanation */
int isnumeric(char *s);

int main (int argc, char *argv[])
{
    /* parse arguments */
    int n = DEFAULT_N; /* if argc == 1 then the default value will stay put */
    if (argc > 3) {
        printf("Unrecognised input: ");
        while (argc--) {
            printf("%s ", *++argv);
        }
        printf("\nUsage: cat file | ./ex513 -n NUMBER, or cat file | ./ex513 -NUMBER\n");
        return 1;
    }
    else if (argc == 3) { /* check for -n NUMBER syntax */
        if (!(strcmp(argv[1], "-n")) && (isnumeric(argv[2]) == NUMBER)) {
            n = atoi(argv[2]);
        }
        else {
            printf("Unrecognised options: ");
            while (argc--) {
                printf("%s ", *++argv);
            }
            printf("\nUsage: cat file | ./ex513 -n NUMBER, or cat file | ./ex513 -NUMBER\n");
            return 1;
        }
    }
    else if (argc == 2) { /* check for -NUMBER syntax */
        if (isnumeric(argv[1]) == MINUS_NUMBER) {
            n = atoi(argv[1] + 1);
        }
        else {
            printf("Unrecognised options: ");
            while (argc--) {
                printf("%s ", *++argv);
            }
            printf("\nUsage: cat file | ./ex513 -n NUMBER, or cat file | ./ex513 -NUMBER\n");
            return 1;
        }
    }
    if (n <= 0) {
        return 1;
    }

    /* I don't like the suggestion in K&R to use the storage method of Section 5.6.
     * That storage is limited and so the program will not work with arbitrarily large files.
     * So here, I have chosen a different way: we have an array of n strings which are sequentially
     * overwritten when a new line is read. A counter (nlines) tells us where the last string
     * written in was. Then the output task is simply to print the n strings in the correct order
     * which we can do easily with the counter. This can be done easily using modulo arithmetic. */

    char lines[n][MAXCHARS]; /* there is still a restriction on maximum line length (my_getline has this restriction too) */
    int i;
    for (i = 0; i < n; i++) {
        strcpy(lines[i], ""); /* initialise everything to empty string, important if input has less than n lines */
        /* I suspect this is better accomplished using malloc, but we haven't learnt it yet. */
    }
    int nlines = 0;
    
    int len;
    while ((len = my_getline(lines[nlines % n], MAXCHARS)) != 0) { /* reads a line in and stores it into lines[nlines % n] */
        nlines++;
    }

    printlines(lines, n, nlines);

    /* By uncommenting the next line we can effectively turn this program into `wc -l`. */
    /* printf("%d\n", nlines); */
    return 0;
}

/* printlines: prints the last n lines of the input */
void printlines (char (*lines)[MAXCHARS], int n, int nlines)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%s", *(lines + ((nlines + i) % n)));
    }
}

/* my_getline: reads in a line and stores it into s, but only if the line is non-empty */
int my_getline(char *s, int lim)
{
    /* The original my_getline would read in the string, but even if the string was empty (i.e. only EOF),
     * it would still store it in the given pointer. This creates problems at the very end of files because
     * the empty string would overwrite one of the lines we wanted to print.
     * The workaround is simple: check first whether the line is empty before storing it into s. */
    char c;
    char read[MAXCHARS];
    int i = 0;
    while ((c = getchar()) != EOF && c != '\n') {
        if (i < lim - 1) {
            read[i++] = c;
        }
    }
    if ((c == '\n') && (i < lim - 1)) {
        read[i++] = c;
    }
    read[i <= lim - 1 ? i : lim - 1] = '\0';
    /* Up to here the code is exactly the same as my_getline.c (well, some minor cosmetic differences).
     * The only difference is the next line. */
    if (i) strcpy(s, read);
    return i;
}

/* isnumeric: checks if a string is a valid number */
/* returns 1 if it fits \d+, 2 if it fits -\d+, 3 if it fits \+\d+, 0 if it fits none */
int isnumeric(char *s)
{
    if (*s == '\0') return 0; /* means it's empty */

    int i = 0; /* the value to be returned */
    if (*s == '-') {
        i = MINUS_NUMBER;
        s++;
    }
    else if (*s == '+') {
        i = PLUS_NUMBER;
        s++;
    }
    else i = NUMBER;

    while (*s) {
        if (!isdigit(*s++)) return 0;
    }
    return i;
}
