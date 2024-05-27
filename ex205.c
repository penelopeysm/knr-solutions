#include <stdio.h>
#define MAXLINE 1000

int any(char s1[], char s2[]);
int my_getline(char s[], int lim);

int main(void){
    char s1[MAXLINE];
    char s2[MAXLINE];
    int len1;
    int len2;

    while ((len1 = my_getline(s1, MAXLINE)) != 0){
        len2 = my_getline(s2, MAXLINE);
        int x = any(s1, s2);
        printf("%d\n", x);
    }

    return 0;
}

int any(char s1[], char s2[]){
    int i = 0;
    while (s1[i] != '\0') {
        int j = 0;
        while (s2[j] != '\0') {
            if (s1[i] == s2[j++]) {
                return i;
            }
        }
        ++i;
    }
    return -1;
}


int my_getline(char s[], int lim) {
    int c, i;
    // c: character to be read
    // i: length of the string

    for (i = 0; ((c = getchar()) != EOF && c != '\n'); ++i) {
    if (i < lim - 1) {
        s[i] = c;
    }
    // sets the i-th character of ths string s to be c
    }

    if ((c == '\n') && (i < lim - 1)) {
    s[i] = c;
    ++i;
    }

    // The code below is inefficient
    /*
    if (i <= lim - 1) {
    s[i] = '\0';
    }
    else {
    s[lim - 1] = '\0';
    }
    */
    // Faster way of doing it
    s[i <= lim - 1 ? i : lim - 1] = '\0';

    return i;
}
