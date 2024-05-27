#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
 
/* ex606 - simple version of #define preprocessor */
/* includes the undef function from ex605 */
/* I tested this and the preprocessed code that it spat out could be compiled again - success! */
/* Actually, an ifdef/ifndef/endif directive should be not too difficult to code because
 * all the bits and pieces are already in place, so it's just about writing a high-level function.
 * But an #else or #elif bit might be difficult */

#define MAXWORD 500

struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE];
struct nlist *lookup(char *);
struct nlist *install(char *, char *);
void uninstall(char *);
unsigned int hash(char *);

char *my_strdup(char *);
int getword(char *, int);
int getch(void);
void ungetch(int);

int main(void)
{
    char word[MAXWORD];
    int c;

    char out[MAXWORD]; /* text to be replaced */
    char in[MAXWORD]; /* text to replace with */

    int lineno; /* current line number */
    lineno = 1;

    struct nlist *tmp;
    
    while ((c = getword(word, MAXWORD)) != EOF) {
        if (c == '\n') {
            lineno++;
            /* printf("\n%d ", lineno); */
        }
        if (strcmp(word, "#define") == 0) { /* found a #define line */
            /* get the text to be replaced */
            while (isspace(c = getch()) && (c != '\n')) ;
            if ((c == '\n') || (c == EOF)) {
                printf("error: no text to be replaced\n");
                return 1;
            }
            else {
                ungetch(c);
                c = getword(out, MAXWORD);
            }
            /* get the replacement text */
            while (isspace(c = getch()) && (c != '\n')) ;
            if ((c == '\n') || (c == EOF)) {
                printf("error: no replacement text defined\n");
                return 1;
            }
            else {
                ungetch(c);
                int lim = MAXWORD;
                char *p = in;
                while ((--lim > 0) && ((c = getch()) != '\n') && (c != EOF)) {
                    *p++ = c;
                }
                while (isspace(*--p)) ; /* backtrack to remove trailing whitespace */
                *++p = '\0';
                ungetch(c);
            }
            /* printf("on line %-5d found name %-10s with hash value %d\n", lineno, out, hash(out)); */
            tmp = install(out, in);
            if (tmp == NULL) {
                printf("error: insufficient memory\n");
                return 1;
            }
            continue; /* don't print the define line */
        }

        else if (strcmp(word, "#undef") == 0) { /* found an #undef line */
            /* get the text to be undefined */
            while (isspace(c = getch()) && (c != '\n')) ;
            if ((c == '\n') || (c == EOF)) {
                printf("error: no text to undefine\n");
                return 1;
            }
            else {
                ungetch(c);
                int lim = MAXWORD;
                char *p = out;
                while ((--lim > 0) && ((c = getch()) != '\n') && (c != EOF)) {
                    *p++ = c;
                }
                while (isspace(*--p)) ; /* backtrack to remove trailing whitespace */
                *++p = '\0';
                ungetch(c);
            }
            uninstall(out);
            continue;
        }

        tmp = lookup(word);
        if (tmp != NULL) {
            printf("%s", tmp->defn); /* spit the definition out */
        }
        else {
            printf("%s", word); /* spit it back out */
        }
    }

    return 0;
}


unsigned int hash(char *s)
{
    unsigned int hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + (31 * hashval);
    return hashval % HASHSIZE;
}

struct nlist *lookup(char *s)
{
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next){
        if (strcmp(s, np->name) == 0){
            return np;
        }
    }
    return NULL;
}

struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned int hashval;
    if ((np = lookup(name)) == NULL) {
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = my_strdup(name)) == NULL) return NULL;
        hashval = hash(name);
        /* These two following lines are quite subtle...
         * Instead of adding a new definition to the end of the line, this effectively
         * inserts the new definition between the hashtab array and the first definition
         * in line. i.e. if we make a new definition c, install() converts
         *    hashtab[hashval] ---> a ---> b  
         * to 
         *    hashtab[hashval] ---> c ---> a ---> b 
         * and not
         *    hashtab[hashval] ---> a ---> b ---> c    */
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    else {
        free((void *) np->defn);
    }
    if ((np->defn = my_strdup(defn)) == NULL) return NULL;
    return np;
}

void uninstall(char *name)
{
    struct nlist *np;
    unsigned int hashval;
    hashval = hash(name);

    if ((np = lookup(name)) == NULL) return; /* silly person tried to undef something that wasn't defined */
    else {
        if (hashtab[hashval] == np) { /* if hashtab[hashval] points directly to np */
            hashtab[hashval] = np->next;
        }
        else { /* find the thing that points to np */
            struct nlist *t = hashtab[hashval];
            while (t->next != np) {
                t = t->next;
            }
            t->next = np->next;
        }
        free((void *) np);
    }
}

/* everything below this should be reliable */

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
    int c, prevc;
    char *w = word;

    c = getch();
    
    if (c != EOF) {
        *w++ = c;
        /* check for character opening */
        if (c == '\'') {
            /* skip until the next '"' that isn't escaped */
            while (--lim > 0) {
                *w++ = c = getch();
                if (c == '\\') {
                    *w++ = c = getch(); /* skip over one more character */
                    lim--;
                    continue;
                }
                if (c == '\'') {
                    *w = '\0';
                    return word[0];
                }
            }
        }
        /* check for string opening */
        if (c == '"') {
            /* skip until the next '"' that isn't escaped */
            while (--lim > 0) {
                *w++ = c = getch();
                if (c == '\\') {
                    *w++ = c = getch(); /* skip over one more character */
                    lim--;
                    continue;
                }
                if (c == '"') {
                    *w = '\0';
                    return word[0];
                }
            }
        }
        /* check for comment opening */
        if (c == '/') {
            /* is it really a comment? */
            if (getch() == '*') {
                int nnewlines = 0;
                /* skip until the next * and / */
                while (1) {
                    prevc = c;
                    c = getch();
                    if (c == '\n') nnewlines++;
                    if ((prevc == '*') && (c == '/'))
                        break;
                    *--w = '\0'; /* by clearing w, we can also preprocess to get rid of comments */
                }
                while (nnewlines--) ungetch('\n');
            }
            /* or maybe it was a pointer */
            else {
                ungetch('*');
                /* the function goes on to return '/' */
            }
        }
    }
    if (!isalpha(c) && c != '#') {
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
