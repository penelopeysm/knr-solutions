#include <stdio.h>

#define SWAP(t, x, y) do { t variable ## x ## y = x ; x = y ; y = variable ## x ## y; } while(0)

int main (void)
{
    int a = 1;
    int b = 2;
    SWAP(int, a, b);
    printf("%d\n", a);
    printf("%d\n", b);
    return 0;
}
