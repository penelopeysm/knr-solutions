#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 200

/* Basic idea: sort words as before, but each word is associated with a struct lineno.
 * Each struct lineno contains a number (corresponding to the line number) as well as
 * a pointer to another struct lineno (the next line number in which the word appears).
 * This strategy avoids the use of fixed-size arrays (memory-inefficient if they are too
 * large and will fail if they are too small). */

struct lineno {
    int line;
    struct lineno *nextline;
} ;

struct tnode {
    char *word;
    struct lineno *firstno;
    struct tnode *left;
    struct tnode *right;
} ;

struct tnode *addtree(struct tnode *, char *, int);
struct lineno *addlineno(struct lineno *, int);
void treeprint(struct tnode *, int);
void lineprint(struct lineno *);
int getword(char *, int);

int main(void)
{
    int curline = 1; /* running line number count */
    int c;
    int maxlength = 0; /* maximum length of word */

    struct tnode *root;
    root = NULL;
    char word[MAXWORD];

    while ((c = getword(word, MAXWORD)) != EOF) {
        if (isalpha(word[0])) {
            root = addtree(root, word, curline);
            maxlength = (maxlength > strlen(word)) ? maxlength : strlen(word);
        }
        if (c == '\n') {
            curline++;
        }
    }
    treeprint(root, maxlength);
    return 0;
}

struct tnode *talloc(void);
struct lineno *qalloc(void);
char *my_strdup(char *);

struct tnode *addtree(struct tnode *p, char *w, int curline)
{
    int cond;
    
    if (p == NULL) { /* new word */
        /* set up a new node */
        p = talloc();
        p->word = my_strdup(w);
        p->firstno = NULL; /* must initialise for addlineno */
        p->firstno = addlineno(p->firstno, curline);
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0) { /* repeated word */
        p->firstno = addlineno(p->firstno, curline);
    }
    else if (cond < 0)
        p->left = addtree(p->left, w, curline);
    else
        p->right = addtree(p->right, w, curline);
    return p;
}

struct lineno *addlineno(struct lineno *q, int curline)
{
    if (q == NULL) { /* line number not counted yet */
        q = qalloc();
        q->line = curline;
        q->nextline = NULL;
    }
    else {
        if (q->line < curline) {
            q->nextline = addlineno(q->nextline, curline);
        }
    }
    return q;

}

void treeprint(struct tnode *p, int maxlength)
{
    if (p != NULL) {
        treeprint(p->left, maxlength);
        printf("%-*s : ", maxlength, p->word);
        lineprint(p->firstno);
        printf("\n");
        treeprint(p->right, maxlength);
    }
}

void lineprint(struct lineno *q)
{
    if (q != NULL) {
        printf("%d", q->line);
        if (q->nextline != NULL) {
            printf(", ");
            lineprint(q->nextline);
        }
    }
}

/* --------------------------------------- */
/* nothing below here needs to be modified */
/* --------------------------------------- */

struct tnode *talloc(void)
{
    return (struct tnode *) malloc(sizeof(struct tnode));
}

struct lineno *qalloc(void)
{
    return (struct lineno *) malloc(sizeof(struct lineno));
}

char *my_strdup(char *s)
{
    char *p;
    p = (char *) malloc(strlen(s)+1);
    if (p != NULL)
        strcpy(p, s);
    return p;
}

int getword(char *word, int lim)
{
    int c, prevc, getch(void);
    int newlines_in_comment;
    void ungetch(int);
    char *w = word;
    
    /* skip over all whitespace except for newlines */
    while (isspace(c = getch()) && c != '\n') ;

    if (c != EOF) {
        *w++ = c;
        /* check for character opening */
        if (c == '\'') {
            /* skip until the next '"' that isn't escaped */
            while (1) {
                c = getch();
                if (c == '\\') {
                    c = getch(); /* skip over one more character */
                    continue;
                }
                if (c == '\'') break;
            }
        }
        /* check for string opening */
        if (c == '"') {
            /* skip until the next '"' that isn't escaped */
            while (1) {
                c = getch();
                if (c == '\\') {
                    c = getch(); /* skip over one more character */
                    continue;
                }
                if (c == '"') break;
            }
        }
        /* check for comment opening */
        if (c == '/') {
            newlines_in_comment = 0; /* must count newlines within comments */
            /* is it really a comment? */
            if (getch() == '*') {
                /* skip until the next * and / */
                while (1) {
                    prevc = c;
                    c = getch();
                    if (c == '\n') {
                        newlines_in_comment++; /* eat them up, but count them */
                    }
                    if ((prevc == '*') && (c == '/')) {
                        while (newlines_in_comment-- > 0) {
                            ungetch('\n'); /* push them back */
                        }
                        break;
                    }
                }
            }
            /* or maybe it was a pointer */
            else {
                ungetch('*');
                /* the function goes on to return '/' */
            }
        }
    }
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for ( ; --lim > 0; w++) {
        if (!isalnum(*w = getch()) && *w != '_') {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}


#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch (void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch (int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
