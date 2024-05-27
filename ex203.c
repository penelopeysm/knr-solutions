#include <stdio.h>
#include <ctype.h>
#define MAXLINE 1000

int htoi(char a[]);
int my_getline(char st[], int max);

int main(void){
    int len; // length of the line
    char line[MAXLINE]; // the line as a string
    long i; // the value returned by htoi
    
    printf("press Ctrl-D to end the program (sends EOF)\n");

    while ((len = my_getline(line, MAXLINE)) != 0){
        i = htoi(line);
        if (i != -1){
            printf("was converted to %ld\n", i);
        }
    }

    return 0;
}

int htoi(char hex[]){
    int i;
    int start;
    if (hex[0] == '0' && tolower(hex[1]) == 'x') start = 2;
    else start = 0;

    long n = 0L;
    for (i = start; hex[i] != '\n'; ++i){
        if (isdigit(hex[i])){
            n = (n * 16) + (hex[i] - '0');
        }
        else if (tolower(hex[i]) >= 'a' && tolower(hex[i]) <= 'f'){
            n = (n * 16) + 10 + (tolower(hex[i]) - 'a');
        }
        else {
            printf("Invalid input.\n");
            return -1;
        }
    }

    return n;
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
