#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLINE 1000
#define DEFAULTSTOP 4
#define MAXSTOPS 50

int isnumeric(char *s) ;
void entab(char *s, char *t, int *tabstops) ;

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


    char *str[] = {"The", "    quick", "        brown", "            fox", "                jumped", "                    over", "                        the", "                            lazy", "                                dog."};
    /* str is a pointer to an array of 13 chars. Right now it points to "The". str+1 would point to "\tquick". */
    /* There are 4n spaces before string number n (counting from zero). */
    char t[MAXLINE];

    for (int i = 0; i < 9; i++) {
        entab(*(str+i), t, tabstops);
        printf("%s\n", t);
    }

    return 0;
}

/* entab: replaces leading spaces with tabs */
/* s: input line;    t: output line;    tabstops: list of tabstops */
void entab(char *s, char *t, int *tabstops)
{
    int spacecount = 0;
    while (*s == ' ') { /* counts the number of leading spaces */
        spacecount++;
        s++;
    }
    while (*tabstops <= spacecount) { /* finds how many tabs are needed */
        *t++ = '\t';
        tabstops++;
    }
    /* find out how many more spaces we need to tack onto the end */
    int remainingspaces = spacecount - *(tabstops - 1); /* -1 because we overshot the tabstops pointer by 1 */
    while (remainingspaces--) {
        *t++ = ' ';
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
