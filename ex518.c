#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
enum { NAME, PARENS, BRACKETS };

void dcl (void);
void dirdcl (void);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

int main (void)
{
    while (gettoken() != EOF) {
        if (tokentype == '\n') continue; /* skip empty input */
        strcpy(datatype, token);
        out[0] = '\0';
        dcl(); /* parse rest of line */
        if (tokentype != '\n') {
            printf("syntax error\n");
            while (gettoken() != '\n') ; /* wait until a newline is found */
            continue; /* skip the last output if input wasn't correct */
        }
        printf("%s: %s %s\n", name, out, datatype);
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
        /* printf("strcpying %s into name\n", token); */
    }
    else {
        printf("Error: expected name or (dcl)\n");
    }
    while ((type = gettoken()) == PARENS || type == BRACKETS) {
        if (type == PARENS) {
            strcat(out, " function returning");
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
    /* printf("calling gettoken\n"); */

    while ((c = getch()) == ' ' || c == '\t') ; /* skip whitespace */
    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            /* printf("returning tokentype PARENS\n"); */
            return tokentype = PARENS;
        }
        else {
            ungetch(c);
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
