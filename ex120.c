#include <stdio.h>

#define TABSTOP 10
#define MAXLINE 1000

int detag_line(char x[], char y[]);

int main(void){
    char line[MAXLINE];
    char line_detagged[MAXLINE];
    int len;

    while ((len = detag_line(line, line_detagged)) != 0){
        printf("%s", line_detagged);
        if (line_detagged[len - 1] != '\n'){
            printf("\n");
        }
    }

    return 0;
}

int detag_line(char line[], char line_detagged[]){
    int len = 0;
    char c;

    // remove tags from the line and replace with spaces
    while (((c = getchar()) != '\n') && c != EOF && len < MAXLINE - TABSTOP){
        
        if (c == '\t'){
            while (len % TABSTOP != 0){
                line_detagged[len] = ' ';
                ++len;
            }
        }
        else {
            line_detagged[len] = c;
            ++len;
        }
    }

    line_detagged[len] = '\0';

    return len;
}
