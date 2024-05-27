#include <stdio.h>
#define MAXLINE 1000

int my_getline(char line[], int maxline);
void copy(char to[], char from[]);

int main(void) {
    int len;
    int max;
    char line[MAXLINE];
    char longest[MAXLINE];

    max = 0;
    while ((len = my_getline(line, MAXLINE)) > 0) {
	if (len > max) {
	    max = len;
	    copy(longest, line);
	}
    }
    
    if (max > 0) {
	printf("The string\n %s\n was %d chars long\n", longest, max);
    }
    return 0;
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

void copy (char to[], char from[]) {
    // copy from into to
    // note that this works on the array itself
    // unlike passing int arguments which modify private (local) copies
    int i = 0;

    while ((to[i] = from [i]) != '\0') {
	++i;
    }

}

