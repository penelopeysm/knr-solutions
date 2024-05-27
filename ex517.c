/* The implementation here is (necessarily) rudimentary. One can either sort entire lines using
 * ./ex517 -ndrf, or sort a field in each line using ./ex517 -i<NUMBER><OPTIONS>. Fields are determined
 * using whitespace as delimiters, so in the line
 *                           somethingbefore 1,2,3,4,5,6,7 somethingafter 
 *                           --------------- ------------- --------------
 * the fields are :              field 1        field 2       field 3  
 *
 * To sort field 2 by numeric order, the command is then ./ex517 -i2n;
 * or in reverse numeric order, the command is ./ex517 -i2nr.
 * Since fields are split by whitespace, there cannot be spaces within a field. This is an obvious flaw.
 * In principle, however, the code can be fairly straightforwardly modified to allow for other delimiters
 * both without and within the field. Between fields, the delimiter is hardcoded via isspace(char), but 
 * inside a field, the delimiter is defined as a variable. This suggests that it is easy enough to create a
 * command-line option to set this delimiter... but at 363 lines of code, I think I've had enough! */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "my_getline.h"

#define MAXLEN 10000
#define MAXLINES 5000000
char *lineptr[MAXLINES];

int parseargs(int c, char **v, int *n, int *r, int *f, int *d, int *i) ;
int readlines (char *lineptr[]) ;
void writelines (char *lineptr[], int nlines) ;

/* int (*comp)(void *, void *) means: a pointer to a function that takes (void *, void *) and returns an int 
 * Conversely: 
 * int *comp(void *, void *) means: a function that takes (void *, void *) and returns a pointer to an int */
void my_qsort (void *lineptr[], int left, int right, int (*comp)(void *, void *, int), int reverse, int fold) ;
int my_strcmp (const char *, const char *, int f) ;
int numcmp (const char *, const char *, int f) ;
int strcmpd (const char *, const char *, int f) ;
int string_split (char *, void *lineptr[], char delim) ;

int main (int argc, char *argv[])
{
    int nlines;    /* number of input lines read */
    int numeric = 0;  /* 1 if numeric sort is desired */
    int reverse = 1;  /* 1 for sorting in ascending order, -1 for descending */
    int fold = 0;     /* 1 for case-insensitive sorting */
    int directory = 0;    /* 1 for directory order, i.e. ignoring special chars */
    int field = 0;    /* 1 for directory order, i.e. ignoring special chars */

    /* parse arguments */
    int a = parseargs(argc, argv, &numeric, &reverse, &fold, &directory, &field);
    if (a) {
        printf("Unrecognised input...\n");
        return 1;
    }

    /* sort the lines... if fields aren't involved */
    if (!field) {
        if ((nlines = readlines(lineptr)) >= 0) {
            my_qsort((void **) lineptr, 0, nlines - 1, (int (*)(void*,void*,int))(numeric ? numcmp : (directory ? strcmpd : my_strcmp)), reverse, fold);
            writelines(lineptr, nlines);
            return 0;
        }
        else {
            printf("Input too big to sort\n");
            return 1;
        }
    }
    else { /* sort within line... if fields are involved */
        int len;
        char s[MAXLEN];
        while ((len = my_getline(s, MAXLEN)) > 0) {
            char *ps = s;
            char before[MAXLEN], tosort[MAXLEN], after[MAXLEN];
            char *pb = before;
            char *pt = tosort;
            char *pa = after;

            /* split the input string into before, tosort, after */
            int nf = 0;
            while (isspace(*ps)) {
                *pb = *ps;
                pb++;
                ps++;
            }
            nf++;
            while (*ps) {
                if (isspace(*ps) && nf < field) {
                    while (isspace(*ps)) {
                        *pb = *ps;
                        pb++;
                        ps++;
                    }
                    nf++;
                }
                else if (!(isspace(*ps)) && nf < field) {
                    while (!(isspace(*ps))) { 
                        *pb = *ps;
                        pb++;
                        ps++;
                    }
                }
                else if (!(isspace(*ps)) && nf == field) {
                    *pb = '\0';
                    while (!(isspace(*ps))) {
                        *pt = *ps;
                        pt++;
                        ps++;
                    }
                    *pt = '\0';
                    nf++;
                }
                else if (nf > field) {
                    while (*ps) {
                        *pa = *ps;
                        pa++;
                        ps++;
                    }
                    *pa = '\0';
                }
            }
            /* split tosort into an array of pointers, replacing the delimiter (currently only commas) with \0 
             * we can reuse lineptr here, I suppose */
            char delimiter = ',';
            int splitlen = 0;
            splitlen = string_split(tosort, (void **) lineptr, delimiter);
            /* qsort the pointers */
            if (splitlen > 0) {
                my_qsort((void **) lineptr, 0, splitlen - 1, (int (*)(void*,void*,int))(numeric ? numcmp : (directory ? strcmpd : my_strcmp)), reverse, fold);
            }
            /* re-form the qsorted list */
            int j;
            char sorted[MAXLEN] = "";
            char delimstr[2] = {delimiter, '\0'};
            for (j = 0; j < splitlen; j++){
                strcat(sorted, lineptr[j]);
                if (j < splitlen - 1) {
                    strcat(sorted, delimstr);
                }
            }
            /* print the qsorted-list, re-joined together with commas. */
            printf("%s%s%s", before, sorted, after);
        }
        return 0;
    }
}

/* string_split: splits up a string, fills lineptr with pointers to the start of each sub-string
 * returns the number of items found in the string
 * Implementation taken mostly from gho.c, but modified to deal with other delimiters */
int string_split(char *s, void *wordptr[], char delimiter)
{
    int nwords = 0; 
    int count = 0; /* counts the number of letters since the start of the last word */ 
    while (*s != '\0') {
        if (*s == delimiter || *s == ' ') {
            *s = '\0';
            if (count > 0) {
                wordptr[nwords++] = s - count;
            }
            count = 0;
        }
        else {
            count++;
        }
        s++;
    }
    /* need to add the last word to the line */
    if (count > 0) {
        wordptr[nwords++] = s - count;
    }
    return nwords;
}

#include <string.h>
#include <stdlib.h>

/* parse command-line arguments */
int parseargs (int argc, char **argv, int *numeric, int *reverse, int *fold, int *directory, int *field)
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
                        case 'd':
                            *directory = 1;
                            break;
                        case 'i':
                            *field = 1; /* the exact value will be set later */
                            goto parsefield;
                        default: /* unrecognised option */
                            return 1;
                    }
                }
            }
            else return 1; /* option that doesn't start with - */
        }
    }

parsefield: 
    /* continue parsing field option
     * the syntax is: -i<NUMBER><OPTIONS> e.g. -i2n sorts the second field by numeric option (fields start from 1, not 0)
     * the fields are essentially Python's line.split(), all whitespace will act as delimiters */
    if (*field) {
        /* skip to after "-i" */
        (*argv)++;
        /* read the number bit into nstr */
        char nstr[100];
        char *p = nstr;
        while (isdigit(**argv)) {
            *p = **argv;
            p++;
            (*argv)++;
        }
        *p = '\0'; /* terminate nstr */
        if (strlen(nstr) > 0) *field = atoi(nstr); /* field now contains the field number */
        else return 1;
        /* reset the other sorting options to their defaults, so that they can be passed back
         * just in case somebody calls it with something like -d -f -i2 ...*/
        *numeric = 0;
        *reverse = 1;
        *fold = 0;
        *directory = 0;
        /* parse the section after the comma */
        char c;
        while ((c = **argv)) {
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
                case 'd':
                    *directory = 1;
                    break;
                default:
                    return 1; /* unrecognised function */
            }
            (*argv)++;
        }
    }
    return 0;
}

/* numcmp: compare two numbers stored as strings s1 and s2 */
int numcmp (const char *s1, const char *s2, int ci)
{
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2) return -1;
    else if (v1 > v2) return 1;
    else return 0;
}

char foldf (char s, int f);
/* my_strcmp: strcmp if fold == 0, insensitive strcmp if fold == 1 */
int my_strcmp (const char *s1, const char *s2, int fold)
{
    /* foldf = tolower if fold == 1, otherwise it does nothing */
    while (foldf(*s1, fold) == foldf(*s2, fold)) {
        if (*s1 == '\0') return 0;
        s1++;
        s2++;
    }
    return (foldf(*s1, fold) - foldf(*s2, fold));
}

/* strcmpd: strcmp but ignoring special characters (only works on whitespace, letters, and numbers).
 *          The parameter fold plays a similar role to that in my_strcmp. */
int strcmpd (const char *s1, const char *s2, int fold)
{
    /* if either string ended */
    if (*s1 == '\0' || *s2 == '\0') return (foldf(*s1, fold) - foldf(*s2, fold));
    /* if both strings haven't ended, skip ahead to the first whitespace/letter/number */
    while (!(isalnum(*s1) || isspace(*s1) || *s1 == '\0')) {
        s1++;
    }
    while (!(isalnum(*s2) || isspace(*s2) || *s2 == '\0')) {
        s2++;
    }
    /* compare the characters here.
     * if they are different, we can end;
     * if they are the same, we advance one character and call strcmpd again */
    if (foldf(*s1, fold) != foldf(*s2, fold)) return (foldf(*s1, fold) - foldf(*s2, fold));
    else {
        s1++;
        s2++;
        return strcmpd(s1, s2, fold);
        /* when the final strcmpd finally returns a value, the strcmpd above it will return _that_ value, and so on */
    }
    return 0; /* It really shouldn't reach here, but otherwise the compiler complains */
}

/* foldf: returns tolower(s) if f = 1, returns s if f = 0 */
char foldf (char s, int f)
{
    return (f ? tolower(s) : s);
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

void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *, int), int reverse, int fold)
{
    int i, last;
    void swap(void *v[], int, int);
    if (left >= right) return;
    swap(v, left, (left + right)/2);
    last = left;
    for (i = left+1; i <= right; i++) {
        if ((*comp)(v[i], v[left], fold) * reverse < 0) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    my_qsort(v, left, last - 1, comp, reverse, fold);
    my_qsort(v, last + 1, right, comp, reverse, fold);
}

void swap (void *v[], int i, int j)
{
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
