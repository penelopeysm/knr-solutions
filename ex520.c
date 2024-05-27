#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Bug: doesn't work with const pointers, e.g.
 *     int * const p;
 * declares a const pointer (read-only pointer) to an int
 * On the bright side, it just says "syntax error" and recovers
 * decently after that. */

#define MAXTOKEN 100
enum { NAME, PARENS, BRACKETS };

void dcl (void);
void dirdcl (void);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char token_args[MAXTOKEN]; /* for function arguments */
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

int main (void)
{
    while (gettoken() != EOF) {
        if (tokentype == '\n') continue; /* skip empty input */
        datatype[0] = '\0'; /* reset it */
        while (!strcmp(token, "const") || !strcmp(token, "static") || !strcmp(token, "extern") || !strcmp(token, "register")) {
            strcat(datatype, token); /* add any qualifiers like const */
            strcat(datatype, " ");
            gettoken();
        }
        strcat(datatype, token); /* add the name of the variable itself */
        out[0] = '\0';
        dcl(); /* parse rest of line */
        if (tokentype != '\n') {
            printf("syntax error\n\n");
            while (gettoken() != '\n') ; /* wait until a newline is found */
            continue; /* skip the last output if input wasn't correct */
        }
        if (*name != '\0') {
            printf("%s: %s %s\n\n", name, out, datatype);
        }
        name[0] = '\0'; /* reset this so that the check in gettoken() works on subsequent uses */
    }
    return 0;
}

/* dcl: parse a declarator */
void dcl (void)
{
    /* printf("calling dcl\n"); */
    int ns;
    for (ns = 0; gettoken() == '*' ; ) { /* count *'s */
        ns++;
    }
    dirdcl();
    while (ns-- > 0) {
        strcat(out, " pointer to");
    }
}

/* dirdcl: parse a direct declarator */
void dirdcl (void)
{
    /* printf("calling dirdcl\n"); */
    int type;
    if (tokentype == '(') {   /* ( dcl ) */
        dcl();
        if (tokentype != ')') {
            printf("Error: missing )\n");
            return; /* stop looking for tokens */
            }
        }
    else if (tokentype == NAME) { /* variable name */
        strcpy(name, token);
    }
    else {
        printf("Error: expected name or (dcl)\n\n");
        return;
    }
    while ((type = gettoken()) == PARENS || type == BRACKETS) {
        if (type == PARENS) {
            strcat(out, " function accepting (");
            strcat(out, token_args);
            strcat(out, ") and returning");
        }
        else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
}

int gettoken (void)
{
    int c, getch(void);
    void ungetch(int);
    char *p = token;
    char *q = token_args;
    /* printf("calling gettoken\n"); */

    while ((c = getch()) == ' ' || c == '\t') ; /* skip whitespace */
    if (c == '(') {
        if (*name != '\0') { /* if name has already been set */
            while ((c = getch()) != ')') {
                *q++ = c;
            }
            /* printf("returning tokentype PARENS\n"); */
            return tokentype = PARENS;
        }
        else { /* if name has not yet been set */
            /* printf("returning tokentype (\n"); */
            return tokentype = '(';
        }
    }
    else if (c == '[') {
        for (*p++ = c; (*p++ = getch()) != ']'; ) ;
        *p = '\0';
        /* printf("returning tokentype BRACKETS\n"); */
        return tokentype = BRACKETS;
    }
    else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch()); )
            *p++ = c;
        *p = '\0';
        ungetch(c);
        /* printf("returning tokentype NAME\n"); */
        return tokentype = NAME;
    }
    else
        /* printf("returning tokentype c = %c\n", c); */
        return tokentype = c;
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
