#include <stdio.h>

void mystrcat(char *s, char *t) ;

int main(void)
{
    char s1[100] = "hello, ";
    char s2[100] = "world!";

    mystrcat(s1, s2);
    printf("%s\n", s1);

    char s3[100] = "hello, ";
    char s4[100] = "everybody!";

    mystrcat(s3, s4);
    printf("%s\n", s3);

    return 0;
}

void mystrcat(char *s, char *t)
{
    while(*s++)
        ;
    s--;
    while((*s++ = *t++))
        ;
}
