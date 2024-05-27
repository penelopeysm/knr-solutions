#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "my_getline.h"

#define MAXLINE 10000
#define MAXWORD 4000

int split(char *s, char *wordptr[]) ;
void qsort(char *v[], int l, int r) ;
void printwords(char *wordptr[], int nwords);

int main()
{
    char s[MAXLINE]; // char array to store the read-in line
    char *wordptr[MAXWORD]; // array of pointers to start of words
    int nwords; // number of words
    int len; // length of line entered

    while (printf(">> ") && (len = my_getline(s, MAXLINE)) != 0) { // printf returns the number of characters printed
        if ((nwords = split(s, wordptr)) > 0) {
            qsort(wordptr, 0, nwords - 1);
            printwords(wordptr, nwords);
        }
    }
    printf("\n");
    return 0;
}

int split(char *s, char *wordptr[])
{
    // places pointers to beginning of words in wordptr
    // returns the number of words as an int
    int nwords = 0; // number of words, to be returned
    int count = 0; // counts the number of letters since the start of the last word
    while (*s != '\0') {
        if (*s == ' ' || *s == '\n') {
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
    return nwords;
}

void qsort(char *v[], int left, int right)
{
    // from K&R 2ed p110
    int i, last;
    void swap(char *v[], int i, int j) ;
    int mystrcmp(char *s, char *t) ;
    if (left >= right)
        return;
    swap(v, left, (left+right)/2);
    last = left;
    for (i = left+1; i <= right; i++)
        if (mystrcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}

void printwords(char *wordptr[], int nwords)
{
    int i;
    printf("\n");
    for (i = 0; i < nwords; i++) {
        printf("%s ", wordptr[i]); // wordptr[i] is a pointer to a string (more accurately, to the first position of the character array) which printf takes
    }
    printf("\n\n");
}

void swap(char *v[], int i, int j)
{
    char *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int mystrcmp(char *s, char *t)
{
    // case-insensitive strcmp
    // returns <0 if s < t (alphabetically), 0 if s == t, >0 if s > t
    for ( ; tolower(*s) == tolower(*t); s++, t++) {
        if (*s == '\0') {
            return 0;
        }
    }
    return tolower(*s) - tolower(*t);
}

