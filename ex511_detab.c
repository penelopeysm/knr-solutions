#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLINE 1000
#define DEFAULTSTOP 4
#define MAXSTOPS 50

int isnumeric(char *s) ;
void detab(char *s, char *t, int *tabstops) ;

int main(int argc, char **argv)
{
    /* parse arguments */
    int tabstops[MAXSTOPS];
    int c = MAXSTOPS;
    int *pt = tabstops;
    if (argc == 1) { /* no tabstops specified */
        while (c--) {
            *pt++ = DEFAULTSTOP * (MAXSTOPS - c);
        }
    }
    else if (argc == 2) { /* one parameter specified, use this as the tab spacing */
        if (isnumeric(argv[1])) {
            int stopsize = atoi(argv[1]);
            while (c--) {
                *pt++ = stopsize * (MAXSTOPS - c);
            }
        }
        else {
            printf("Unrecognised input!");
        }
    }
    else if (argc > 2) { /* n (> 1) parameters specified */
        char *argi;
        int leftover = MAXSTOPS - argc + 1; /* number of tabstops we have to fill afterwards */
        while (--argc) { /* fill up the first n tabstops */
            argi = *++argv;
            if (isnumeric(argi)) {
                *pt++ = atoi(argi);
            }
            else {
                printf("Unrecognised input!");
            }
        }
        while (leftover--) { /* fill up the rest of the tabstops with default spacing */
            int j = *(pt-1);
            *pt++ = j + DEFAULTSTOP;
        }
    }


    char *str[] = {"The", "\tquick", "\t\tbrown", "\t\t\tfox", "\t\t\t\tjumped", "\t\t\t\t\tover", "\t\t\t\t\t\tthe", "\t\t\t\t\t\t\tlazy", "\t\t\t\t\t\t\t\tdog."};
    /* str is a pointer to an array of 13 chars. Right now it points to "The". str+1 would point to "\tquick". */
    char t[MAXLINE];

    for (int i = 0; i < 9; i++) {
        detab(*(str+i), t, tabstops);
        printf("%s\n", t);
    }

    return 0;
}

/* detab: replaces leading tabs with spaces */
/* s: input line;    t: output line;    tabstops: list of tabstops */
void detab(char *s, char *t, int *tabstops)
{
    int spacecount = 0;
    while (*s == '\t') { /* replaces leading tabs with spaces */
        while (spacecount < *tabstops) {
            *t++ = ' ';
            spacecount++;
        }
        s++;
        tabstops++;
    }
    while ((*t = *s)) { /* copies the rest of the string */
        s++;
        t++;
    }
}

/* isnumeric: checks if a string is a valid number */
/* returns 1 if it is, 0 if it isn't */
int isnumeric(char *s)
{
    if (*s == '\0') return 0; // means it's empty
    while (*s) {
        if (!isdigit(*s++)) return 0;
    }
    return 1;
}
