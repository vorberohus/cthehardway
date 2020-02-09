#include <stdio.h>

int main(int argc, char *argv[])
{
    int ints[] = {1, 2, 3, 4 ,5};
    char name[] = {'A', 'l', 'i', 'c', 'e'};

    printf("Name is %s sizeof is %ld\n", name, sizeof(name));
    printf("sizeof ints is %ld\n", sizeof(ints));
    
    int a = 1;
    int b = 2;

    int c = (a += 2, b=a+b, a+b);

    printf("The value of c is: %d\n", c);

    return 0;
}
