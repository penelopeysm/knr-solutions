#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXLINE 1000

int my_getline(char s[], int lim) ;
int my_binsearch(int x, int v[], int n) ;
int binsearch(int x, int v[], int n) ;

int main(void)
{
    // initialise array = 0 to 99
    int n = 100;
    int v[n];
    for (int i = 0; i < n; ++i) {
        v[i] = i;
    }

    int len;
    char s[MAXLINE];
    while ((len = my_getline(s, MAXLINE)) != 0) {
        int deci = atoi(s);
        // modified version
        clock_t start_mod = clock();
        int my_result = my_binsearch(deci, v, n);
        clock_t end_mod = clock();
        printf("my_binsearch(%d,v,n) = %d\n", deci, my_result);
        printf("my_binsearch took %f\n", ((double) (end_mod - start_mod) / CLOCKS_PER_SEC));
        // unmodified version
        clock_t start = clock();
        int result = binsearch(deci, v, n);
        clock_t end = clock();
        printf("binsearch(%d,v,n) = %d\n", deci, result);
        printf("binsearch took %f\n", ((double) (end - start) / CLOCKS_PER_SEC));
    }
    return 0;
}

int my_binsearch(int x, int v[], int n)
{
    int low, high, mid;
    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x < v[mid]) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    if (x == v[mid]) {
        return mid;
    }
    else {
        return -1;
    }
}

int binsearch(int x, int v[], int n)
{
    int low, high, mid;
    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x < v[mid]) high = mid - 1;
        else if (x > v[mid]) low = mid + 1;
        else return mid;
    }
    return -1;
}

int my_getline(char s[], int lim)
{
    int c, i;
    for (i = 0; ((c = getchar()) != EOF && c != '\n'); ++i) {
        if (i < lim - 1) {
            s[i] = c;
        }
    }
    if ((c == '\n') && (i < lim - 1)) {
        s[i] = c;
        ++i;
    }
    s[i <= lim - 1 ? i : lim - 1] = '\0';
    return i;
}
