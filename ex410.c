#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "my_getline.h"

#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define VARIABLE '1'
#define OTHER_OPERATION '2'
#define END_OF_FILE '3'
#define NEWLINE '4'

int nopop = 0;
double most_recent;
double var[26];

int getop(char [], char []);
void push(double);
double pop(void);
void print_top(void);
void duplicate_top(void);
void swap_top_two(void);
void clear_stack(void);
void print_stack(void);

#define MAXLINE 100

/* reverse Polish calculator */

int main(void)
{
    int type;
    double op2;
    char l[MAXLINE];
    char s[MAXOP];
    int var_no;
    double var_val;
    int len;

    printf("define lowercase single-letter variables with \"a 2 def\";\n");
    printf("the variable \"A\" stores the last shown answer\n");

    while((len = my_getline(l, MAXLINE)) != 0)
    {
        while((type = getop(l, s)) != END_OF_FILE)
        {
            switch(type)
            {
                case NUMBER:
                    push(atof(s));
                    break;
                case VARIABLE:
                    if (islower(s[0])) {
                        var_no = (s[0] - 'a');
                        push(var[var_no]);
                    }
                    else if (s[0] == 'A') {
                        push(most_recent);
                    }
                    break;
                case OTHER_OPERATION:
                    if (!(strcmp(s, "sin"))) { // strcmp returns 0 if the strings match
                        push(sin(pop()));
                    }
                    else if (!(strcmp(s, "cos"))) {
                        push(cos(pop()));
                    }
                    else if (!(strcmp(s, "tan"))) {
                        push(tan(pop()));
                    }
                    else if (!(strcmp(s, "exp"))) {
                        push(exp(pop()));
                    }
                    else if (!(strcmp(s, "pow"))) {
                        op2 = pop();
                        push(pow(pop(), op2));
                    }
                    else if (!(strcmp(s, "sqrt"))) {
                        push(sqrt(pop()));
                    }
                    else if (!(strcmp(s, "def"))) {
                        double var_val = pop();
                        var[var_no] = var_val;
                        pop();
                        /* when calling "a 2 def" for example, 'a' pushes the value of a to the stack
                         * then 2 is pushed, so overall we need to pop 2 off (first line) and then pop
                         * the original value of a off as well. */
                        // Because of initialisation, any undefined variables have a value of 0.
                        nopop = 1;
                    }
                    else {
                        printf("error: unknown command %s\n", s);
                    }
                    break;
                case NEWLINE:
                    if (!nopop) {
                        most_recent = pop();
                        printf("\t%.8g\n", most_recent);
                    }
                    nopop = 0;
                    break;
                case '+':
                    push(pop() + pop());
                    break;
                case '*':
                    push(pop() * pop());
                    break;
                case '-':
                    op2 = pop();
                    push(pop() - op2);
                    break;
                case '/':
                    op2 = pop();
                    if(op2 != 0.0)
                        push(pop() / op2);
                    else
                        printf("error: zero divisor\n");
                    break;
                case '%':
                    op2 = pop();
                    if (op2 != 0.0)
                        push(((int) pop()) % ((int) op2));
                    else
                        printf("error: zero divisor\n");
                    break;
                case '@':
                    print_top();
                    nopop = 1;
                    break;
                case '#':
                    duplicate_top();
                    nopop = 1;
                    break;
                case '$':
                    swap_top_two();
                    nopop = 1;
                    break;
                case '^':
                    clear_stack();
                    nopop = 1;
                    break;
                case '!':
                    print_stack();
                    nopop = 1;
                    break;
                default:
                    printf("error: unknown command %s\n", s);
                    break;
            }
        }
    }

    return 0;
}

#define MAXVAL  100 /* maximum depth of val stack */

int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */

/* push: push f onto value stack */
void push(double f)
{
    if(sp < MAXVAL) {
        val[sp++] = f;
    }
    else
        printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
    if(sp > 0)
        return val[--sp];
    else
    {
        printf("error: stack empty\n");
        return 0.0;
    }
}

/* print_top: print the top value without popping it */
void print_top(void)
{
    if (sp > 0) {
        printf("top element: %g\n", val[sp - 1]);
    }
    else {
        printf("error: stack empty\n");
    }
}

/* duplicate_top: duplicate top element of stack */
void duplicate_top(void)
{
    if (sp > 0) {
        ++sp;
        val[sp - 1] = val[sp - 2];
    }
    else {
        printf("error: stack empty\n");
    }
}
    
/* swap_top_two: swap top two elements in stack */
void swap_top_two(void)
{
    if (sp > 1) {
        double temp;
        temp = val[sp - 1];
        val[sp - 1] = val[sp - 2];
        val[sp - 2] = temp;
    }
    else {
        printf("error: stack has less than two items\n");
    }
}

/* clear_stack: clears the stack */
void clear_stack(void)
{
    sp = 0;
}

void print_stack(void)
{
    for (int i = 0; i < sp; ++i) {
        printf("%g ", val[i]);
    }
    printf("\n");
}

#include <ctype.h>

int getop(char f[], char s[])
{
    // getop parses the string f (f should be obtained from my_getline()) and
    // places the first operator / operand into s. It then needs to slice off
    // s from the beginning of f.

    // first we need to check if there's anything inside apart from a newline
    if (f[0] == '\n') {
        f[0] = '\0';
        return NEWLINE;
    }
    if (f[0] == EOF) {
        f[0] = '\0';
        return END_OF_FILE;
    }

    int i; // the index of f
    char c; // the character being read
    i = 0;
    while ((c = f[i]) == ' ' || c == '\t') i++ ; // skip whitespace

    int j; // index of s being written to
    j = 0;
    while ((c = f[i]) != ' ' && c != '\t' && c != '\n' && c != '\0' && c != EOF) {
        s[j] = c;
        i++;
        j++;
    }
    s[j] = '\0'; // terminate the string

    int k; // the index of f being overwritten
    k = 0;
    while ((c = f[i]) != '\0') {
        f[k] = c;
        i++;
        k++;
    }
    f[k] = '\0'; // terminate the overwritten string

    // now f and s should be set correctly
    // we now need to determine what value getop() should return to the main routine

    // if s is empty, it means we reached the end of the line
    if (strlen(s) == 0) return END_OF_FILE;

    // if s is an alphabetical string
    if (isalpha(s[0])) {
        if (strlen(s) == 1) return VARIABLE;
        else return OTHER_OPERATION;
    }

    // if s is an operator (one character long...)
    if (strlen(s) == 1) {
        if (isdigit(s[0])) return NUMBER;
        else return s[0];
    }
    
    // otherwise it's a number
    return NUMBER;
}

int my_getline(char s[], int lim)
{
    int c, i;
    for (i = 0; ((c = getchar()) != EOF && c != '\n'); ++i) {
        if (i < lim - 1) {
            s[i] = c;
        }
    }
    if ((c == '\n') && (i < lim - 1)) {
        s[i] = c;
        ++i;
    }
    s[i <= lim - 1 ? i : lim - 1] = '\0';
    return i;
}
