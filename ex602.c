#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* Basic idea: we sort words as before, but we also add a (essentially Boolean) flag
 * which tells the programme whether the word should be printed or not. This flag is
 * set by default to 0, but if we find a word that has the same first six letters, then
 * it is set to 1. */

#define MAXWORD 200
#define DEFAULT_SEARCH_LENGTH 6

struct tnode {
    char *word;
    int print;
    struct tnode *left;
    struct tnode *right;
} ;

struct tnode *addtree(struct tnode *, char *, int toprint, int ncomp) ;
void treeprint(struct tnode *) ;
int getword(char *, int) ;

int main(int argc, char **argv)
{
    int ncomp = DEFAULT_SEARCH_LENGTH; /* number of letters to compare, default 6 */
    int i = 1;
    char name[MAXWORD];
    while (--argc > 0) {
        strcpy(name, *argv++); /* skip over argv[0] */
        if (**argv == '-') {
            (*argv)++; /* move forward one character */
            printf("%s\n", *argv);
            if (strstr("n", *argv)) {
                if (argc > 0) {
                    argc--; /* jump to next argument */
                    argv++;
                    i = atoi(*argv);
                    ncomp = (i > 0) ? i : DEFAULT_SEARCH_LENGTH;
                }
                else {
                    printf("usage: %s [-h] [-n NUM]\n", name);
                }
            }
            else if (strstr("help", *argv)) {
                printf("usage: %s [-h] [-n NUM]\n", name);
                return 0;
            }
        }
    }

    printf("ncomp is %d\n", ncomp);

    struct tnode *root;
    root = NULL;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            root = addtree(root, word, 0, ncomp);
        }
    }
    treeprint(root);
    return 0;
}

int abcdefg;
int abcdefh;
int abcdefi;
int abcdefj;
int abcdefjk;
int abcdegj; /* abcdegl - this should not be printed */

struct tnode *talloc(void);
char *my_strdup(char *);

struct tnode *addtree(struct tnode *p, char *w, int toprint, int ncomp)
{
    int cond;
    
    if (strlen(w) >= ncomp) {
        if (p == NULL) { /* new word */
            /* set up a new node */
            p = talloc();
            p->word = my_strdup(w);
            p->print = toprint;
            p->left = p->right = NULL;
        }
        else if ((cond = strcmp(w, p->word)) == 0) { /* repeated word */
            ; /* nothing to do */
        }
        else { /* the word is different from something already in the tree */
            /* check if the first six letters are the same */
            if (strncmp(w, p->word, ncomp) == 0) {
                p->print = 1; /* enable printing for both words */
                toprint = 1;
            }
            /* then recursively make the new node */
            if (cond < 0) {
                p->left = addtree(p->left, w, toprint, ncomp);
            }
            else if (cond > 0) {
                p->right = addtree(p->right, w, toprint, ncomp);
            }
        }
    }
    return p;
}

void treeprint(struct tnode *p)
{
    if (p != NULL) {
        treeprint(p->left);
        if (p->print == 1) { /* only actually print if the flag is turned on */
            printf("%s\n", p->word);
        }
        treeprint(p->right);
    }
}

/* --------------------------------------- */
/* nothing below here needs to be modified */
/* --------------------------------------- */

struct tnode *talloc(void)
{
    return (struct tnode *) malloc(sizeof(struct tnode));
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
    void ungetch(int);
    char *w = word;
    
    while (isspace(c = getch())) ;

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
            /* is it really a comment? */
            if (getch() == '*') {
                /* skip until the next * and / */
                while (1) {
                    prevc = c;
                    c = getch();
                    if ((prevc == '*') && (c == '/'))
                        break;
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
