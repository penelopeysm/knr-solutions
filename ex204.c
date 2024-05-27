#include <stdio.h>
#define MAXLINE 1000

// Is there a more efficient way to do this rather than using nested for loops?
// Should search when I get back home

void squeeze(char s1[], char s2[]);
int my_getline(char s[], int lim);

int main(void){
    char s1[MAXLINE];
    char s2[MAXLINE];
    int len1;
    int len2;
    char squeezed[MAXLINE];

    while ((len1 = my_getline(s1, MAXLINE)) != 0){
        len2 = my_getline(s2, MAXLINE);
        squeeze(s1, s2);
        printf("%s\n", s1);
    }

    return 0;
}

void squeeze(char s1[], char s2[]){
    int i;
    int j;
    int k;

    for (i = j = 0; s1[i] != '\0'; ++i){
        int print = 1; // 0 if a match is found
        for (k = 0; s2[k] != '\0'; ++k){
            if (s1[i] == s2[k]) {
                print = 0;
            }
        }
        if (print) s1[j++] = s1[i];
    }
    s1[j] = '\0';
}


int my_getline(char s[], int lim) {
    int c, i;
    // c: character to be read
    // i: length of the string

    for (i = 0; ((c = getchar()) != EOF && c != '\n'); ++i) {
    if (i < lim - 1) {
        s[i] = c;
    }
    // sets the i-th character of the string s to be c
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
