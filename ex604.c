#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 200
#define DEFAULT_SEARCH_LENGTH 6

/* It would be easier to pipe the output of the original word-counting programme into
 * sort -nr, but I assume that's not allowed, so we need to implement quicksort here... */

struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
} ;

struct tnode *addtree(struct tnode *, char *);
void makearray(struct tnode *, struct tnode **);
int tnodecmp(const void *, const void *);
int getword(char *, int);

int nwords = 0; /* global */
int nnodes = 0; /* global */

int main(void)
{
    struct tnode *root;
    root = NULL;
    char word[MAXWORD];

    /* generate the tree */
    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            root = addtree(root, word);
        }
    }
    /* nwords tells us how many words in total there are, so we know
     * how much memory to allocate. */
    struct tnode **roottable;
    roottable = (struct tnode **) malloc(sizeof(struct tnode *) * nwords);

    /* printf("nwords = %d\n", nwords); */
    /* populate roottable with the entries of root */
    makearray(root, roottable);
    /* printf("nnodes = %d\n", nnodes); */

    /* sort the roottable array using stdlib's qsort */
    /* Note that here we're sorting an array of pointers to struct.
     * The first argument of qsort is a pointer to the first element (which is the same
     * as the array name).
     * The second argument is the length of the array.
     * The third argument is the size of an array element (i.e. the size of a pointer to struct).
     * The fourth argument, the comparator function, compares __pointers to the array elements__, 
     * i.e. pointers to pointers to struct. So, we need to be very careful when writing tnodecmp. 
     * see https://stackoverflow.com/questions/23689687/sorting-an-array-of-struct-pointers-using-qsort */
    qsort(roottable, nwords, sizeof(struct tnode *), tnodecmp);

    /* finally, print them in order */
    int i;
    for (i = 0; i < nwords; i++) {
        printf("%4d %s\n", (*(roottable+i))->count, (*(roottable+i))->word);
    }

    return 0;
}

void makearray(struct tnode *p, struct tnode **roottable)
{
    if (p != NULL) {
        makearray(p->left, roottable);
        *(roottable + nnodes) = p;
        nnodes++;
        makearray(p->right, roottable);
    }
}

int tnodecmp(const void *pp, const void *qq)
{
    /* as described above, pp and qq are pointers to pointers to struct.
     * to get back the original pointers to struct, we need to cast and then dereference them. */

    /* Note that we are sorting here both by count, as well as by alphabetical order.
     * If we just sort by count, then the alphabetical order of words with the same frequency gets
     * scrambled, because qsort is not stable. Another remedy is to use mergesort instead of qsort. */
    struct tnode *p = *(struct tnode **)pp;
    struct tnode *q = *(struct tnode **)qq;
    if (p->count == q->count) { /* compare words using case-insensitive strcmp */
        char *pw = p->word;
        char *qw = q->word;
        while (tolower(*pw) == tolower(*qw)) {
            pw++;
            qw++;
            if (*pw == '\0') return 0; /* in case the strings are the same, but they really shouldn't be */
        }
        return tolower(*pw) - tolower(*qw);
    }
    else {
        return q->count - p->count; /* because we want to sort from large to small */
    }
}

/* --------------------------------------- */
/* nothing below here needs to be modified */
/* --------------------------------------- */

struct tnode *talloc(void);
char *my_strdup(char *);

struct tnode *addtree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL) { /* new word */
        /* set up a new node */
        nwords++; /* increment global */
        p = talloc();
        p->word = my_strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0) { /* repeated word */
        p->count++;
    }
    else if (cond < 0) { /* the word is different from something already in the tree */
        p->left = addtree(p->left, w);
    }
    else {
        p->right = addtree(p->right, w);
    }
    return p;
}

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
