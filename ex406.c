#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define VARIABLE '1'
#define OTHER_OPERATION '2'

int nopop = 0;
double most_recent;
double var[26];

int getop(char []);
void push(double);
double pop(void);
void print_top(void);
void duplicate_top(void);
void swap_top_two(void);
void clear_stack(void);
void print_stack(void);

/* reverse Polish calculator */

int main(void)
{
    int type;
    double op2;
    char s[MAXOP];
    int var_no;
    double var_val;

    printf("define lowercase single-letter variables with \"a 2 def\";\n");
    printf("the variable \"A\" stores the last shown answer\n");

    while((type = getop(s)) != EOF)
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
            case '\n':
                if (!nopop) {
                    most_recent = pop();
                    printf("\t%.8g\n", most_recent);
                }
                nopop = 0;
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

    return 0;
}

#define MAXVAL  100 /* maximum depth of val stack */

int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */

/* push: push f onto value stack */
void push(double f)
{
    if(sp < MAXVAL)
        val[sp++] = f;
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

int getch(void);
void ungetch(int);

/* getop: get next operator or numeric operand */
int getop(char s[])
{
    int i, c;

    while((s[0] = c = getch()) == ' ' || c == '\t')
        ;

    s[1] = '\0';

    i = 0;

    if (isalpha(c)) {
        while (isalpha(s[++i] = c = getch()))
            ;
        if (c != EOF){
            ungetch(c);
        }
        s[i] = '\0';
        if (i == 1) {
            return VARIABLE;
        }
        else {
            return OTHER_OPERATION;
        }
    }

    if(!isdigit(c) && c != '.' && c != '+' && c != '-'){
        return c; /* not a number, or a sign */
    }

    int sign;
    if ((sign = c) == '-' || c == '+'){
        s[++i] = c = getch();
        if(!isdigit(c) && c != '.'){
            s[i] = '\0';
            ungetch(c);
            return sign; /* not a number */
        }
        else {
            if(isdigit(c)) /* collect integer part */
                while(isdigit(s[++i] = c = getch()))
                    printf("debug2");
            if(c == '.')
                while(isdigit(s[++i] = c = getch()))
                    ;
        }
    }
    else {
        if(isdigit(c)) /* collect integer part */
            while(isdigit(s[++i] = c = getch()))
                ;
        if(c == '.')
            while(isdigit(s[++i] = c = getch()))
                ;
    }
    s[i] = '\0';
    if(c != EOF)
        ungetch(c);
    return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

int getch(void) /* get a (possibly pushed back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if(bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
