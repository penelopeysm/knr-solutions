#include <stdio.h>
#include <string.h>
#include "my_getline.h"

#define MAXLINES 5000000
char *lineptr[MAXLINES];

int parseargs(int c, char **v, int *n, int *r, int *f) ;
int readlines (char *lineptr[]) ;
void writelines (char *lineptr[], int nlines) ;

/* int (*comp)(void *, void *) means: a pointer to a function that takes (void *, void *) and returns an int 
 * Conversely: 
 * int *comp(void *, void *) means: a function that takes (void *, void *) and returns a pointer to an int */
void my_qsort (void *lineptr[], int left, int right, int (*comp)(void *, void *), int reverse) ;
int istrcmp (const char *, const char *) ;
int numcmp (const char *, const char *) ;

int main (int argc, char *argv[])
{
    int nlines;    /* number of input lines read */
    int numeric = 0;  /* 1 if numeric sort is desired */
    int reverse = 1;  /* 1 for sorting in ascending order, -1 for descending */
    int fold = 0;     /* 1 for case-insensitive sorting */

    /* parse arguments */
    int a = parseargs(argc, argv, &numeric, &reverse, &fold);
    if (a) {
        printf("Unrecognised input...\n");
        return 1;
    }

    /* sort */
    if ((nlines = readlines(lineptr)) >= 0) {
        my_qsort((void **) lineptr, 0, nlines - 1, (int (*)(void*,void*))(numeric ? numcmp : (fold ? istrcmp : strcmp)), reverse);
        writelines(lineptr, nlines);
        return 0;
    }
    else {
        printf("Input too big to sort\n");
        return 1;
    }
}

/* parse command-line arguments */
int parseargs (int argc, char **argv, int *numeric, int *reverse, int *fold)
{
    if (argc > 1) {
        char c;
        while (--argc) {
            if (**(++argv) == '-') {
                while ((c = *++(*argv))) {
                    switch (c) {
                        case 'n':
                            *numeric = 1;
                            break;
                        case 'r':
                            *reverse = -1;
                            break;
                        case 'f':
                            *fold = 1;
                            break;
                        default: /* unrecognised option */
                            return 1;
                    }
                }
            }
            else return 1; /* option that doesn't start with - */
        }
    }
    return 0;
}

#include <stdlib.h>
#define MAXLEN 10000

/* numcmp: compare two numbers stored as strings s1 and s2 */
int numcmp (const char *s1, const char *s2)
{
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2) return -1;
    else if (v1 > v2) return 1;
    else return 0;
}

#include <ctype.h>
/* istrcmp: case-insensitive version of strcmp */
int istrcmp (const char *s1, const char *s2)
{
    while (tolower(*s1) == tolower(*s2)) {
        if (*s1 == '\0') return 0;
        s1++;
        s2++;
    }
    return (tolower(*s1) - tolower(*s2));
}

int readlines(char *lineptr[])
{
    int len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = my_getline(line, MAXLEN)) > 0)
        if (nlines >= MAXLINES || (p = (char *) malloc(len * sizeof(char))) == NULL){
            return -1; /* insufficient memory */
        }
        else {
            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    return nlines;
}

void writelines(char *lineptr[], int nlines)
{
    int i;
    for (i = 0; i < nlines; i++) {
        printf("%s\n", lineptr[i]);
        free(lineptr[i]);
    }
}

void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *), int reverse)
{
    int i, last;
    void swap(void *v[], int, int);
    if (left >= right) return;
    swap(v, left, (left + right)/2);
    last = left;
    for (i = left+1; i <= right; i++) {
        if ((*comp)(v[i], v[left]) * reverse < 0) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    my_qsort(v, left, last - 1, comp, reverse);
    my_qsort(v, last + 1, right, comp, reverse);
}

void swap (void *v[], int i, int j)
{
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

