#include <stdio.h>

#define SIZE 100

void mystrncpy(char *dest, char *source, int n) ;
void mystrncat(char *dest, char *source, int n) ;
int mystrncmp(char *s, char *t, int n) ;

int main(void)
{
    char s[SIZE] = "hello world!";
    char t[SIZE] = "testingdef";
    char u[SIZE] = "testingabc";

    int i = 5;
    int j = 8;

    printf("mystrncmp(%s, %s, %d) = %d\n", t, u, i, mystrncmp(t, u, i));
    printf("mystrncmp(%s, %s, %d) = %d\n", t, u, j, mystrncmp(t, u, j));
    printf("mystrncmp(%s, %s, %d) = %d\n", s, u, i, mystrncmp(s, u, i));
    printf("mystrncpy(%s, %s, %d) -> ", t, u, j);
    mystrncpy(t, u, j);
    printf("%s\n", t);
    printf("mystrncat(%s, %s, %d) -> ", t, u, i);
    mystrncat(t, u, i);
    printf("%s\n", t);
    return 0;
}

void mystrncpy(char *dest, char *source, int n)
{
    int i;
    for (i = 0; (i < n) && (*(dest+i) = *(source+i)); i++)
        ;
    *(dest+i) = '\0';
    // doesn't have the exact same behaviour as library mystrncpy, but it's ok
}

void mystrncat(char *dest, char *source, int n)
{
    int i, j;
    for (i = 0; *(dest+i); i++)
        ;
    for (j = 0; (j < n) && (*(dest+i+j) = *(source+j)); j++)
        ;
}

int mystrncmp(char *s, char *t, int n)
{
    int i;
    for (i = 0; (*(s+i) == *(t+i)) && i < n; i++)
        if (!(*(s+i)))
            return 0;
    return *(s+i) - *(t+i);
}
