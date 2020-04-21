#include <stdio.h>

static const int N = 1 << 10;

int main()
{
    printf("%d\n", N & 63);
    return 0;
}