#include <stdio.h>
#include <stdlib.h> // for atoi
#include <ctype.h> // for isdigit
#include <string.h> // for strcmp

int isnumeric(char *s);
int push(int val) ;
int pop() ; 

/* expr: evaluates reverse Polish expression from command line */
int main(int argc, char *argv[])
{
    int i, j;
    while (--argc > 0) {
        char *s = *++argv; // s is argv[1], [2], ...
        if (isnumeric(s)) {
            push(atoi(s));
        }
        else if (!strcmp(s, "+")) {
            i = pop();
            j = pop();
            push(i+j);
        }
        else if (!strcmp(s, "-")) {
            i = pop();
            j = pop();
            push(j-i);
        }
        else if (!strcmp(s, "*")) {
            i = pop();
            j = pop();
            push(i*j);
        }
        else if (!strcmp(s, "/")) {
            i = pop();
            j = pop();
            push(j/i);
        }
        else {
            printf("Unsupported input: %s\n", s);
            return -1;
        }
    }
    // pop the top value and print that as the final answer
    printf("ans: %d\n", pop());
    return 0;
}


/* isnumeric: checks if a string is a valid number */
/* returns 1 if it is, 0 if it isn't */
int isnumeric(char *s)
{
    if ((*s == '+') || (*s == '-')) s++;
    if (*s == '\0') return 0; // means it's just a + or -
    while (*s) {
        if (!isdigit(*s++)) return 0;
    }
    return 1;
}

#define STACKSIZE 100
static int stack[STACKSIZE]; // the stack for the calculator
static int *stackp = stack; // pointer to the first free position of the stack

/* push: pushes a value to the topmost free position */
/* returns -1 if the stack is full, 0 otherwise */
int push(int val)
{
    if (stackp > stack + STACKSIZE) {
        return -1;
    }
    else {
        *stackp++ = val;
        return 0;
    }
}

/* pop: returns the topmost value of the stack */
int pop()
{
    if (stackp > stack) {
        return *--stackp;
    }
    else {
        printf("Stack empty\n");
        return 0;
    }
}
