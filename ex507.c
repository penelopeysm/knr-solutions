#include <stdio.h>
#include <string.h>
#define MAXLINES 5000
#define ALLOCSIZE 10000

char *lineptr[MAXLINES]; /* pointers to the text lines */

int readlines(char *lineptr[], int nlines, char buf[], char *allocp) ;
void writelines(char *lineptr[], int nlines) ;
void qsort(char *lineptr[], int left, int right) ;

/* sort input lines */
int main()
{
    int nlines;
    char allocbuf[ALLOCSIZE];
    char *allocp = allocbuf;

    if ((nlines = readlines(lineptr, MAXLINES, allocbuf, allocp)) >= 0) {
        qsort(lineptr, 0, nlines-1);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("error: input too big to sort\n");
        return 1;
    }
}

#define MAXLEN 1000
#include "my_getline.h"
char *alloc(int len, char buf[], char *allocp);

/* readlines: read input lines */
int readlines(char *lineptr[], int maxlines, char buf[], char *allocp)
{
    int len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = my_getline(line, MAXLEN)) > 0)
        if (nlines >= maxlines || (p = alloc(len, buf, allocp)) == NULL)
            return -1;
        else {
            line[len - 1] = '\0'; // remove trailing newline & signals end of string
            strcpy(p, line); // stores line in allocbuf
            lineptr[nlines++] = p;
            allocp += len;
        }
    return nlines;
}

/* writelines: write output lines */
void writelines(char *lineptr[], int nlines)
{
    int i;
    for (i = 0; i < nlines; i++)
        printf("%s\n", lineptr[i]);
}

/* qsort: sort v[left] ... v[right] in ascending order */
void qsort(char *v[], int left, int right)
{
    int i, last;
    void swap(char *v[], int i, int j) ;

    if (left >= right)
        return;
    swap(v, left, (left+right)/2);
    last = left;
    for (i = left+1; i <= right; i++)
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}

/* swap: interchange v[i] and v[j] */
void swap(char *v[], int i, int j)
{
    char *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

char *alloc(int n, char allocbuf[], char *p)
{
    if (allocbuf + ALLOCSIZE - p >= n) { // it fits
        p += n;
        return p - n;
    } else // it doesn't fit
        return 0;
}
