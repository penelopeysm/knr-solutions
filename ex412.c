#include <stdio.h>
#define MAXCHAR 1000

void itoa (int x, char s[]) ;

int main (void)
{
    int numbers[] = {-100, -40, 0, 30, 20, 31, 3948, -49, 0, 84828};
    char str[MAXCHAR] = "";

    for (int i = 0; i < 10; i++) {
        itoa(numbers[i], str);
        printf("%s\n", str);
    }
    
    return 0;
}

void itoa (int x, char s[])
{
    static int recursive = 0; // this will be 1 if itoa is called by itself

    if (!recursive) {
        s[0] = '\0'; // empties the string first
    }

    if (x < 0){
        s[0] = '-';
        s[1] = '\0';
        x = -x;
    }

    if (x / 10){ // equivalent to if x >= 10
        recursive = 1;
        itoa(x / 10, s);
    }

    int pos = 0;
    while (s[pos] != '\0') pos++; // skip to the end of the string
    s[pos] = (x % 10) + '0';
    s[pos + 1] = '\0';
    recursive = 0; // this line will only be executed when x is less than 10
}
