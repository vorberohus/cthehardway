#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("No arguments.\n");
    }
    else if(argc == 2)
    {
        printf("Only one argument.\n");
    }
    else if(argc > 2 && argc < 5)
    {
        printf("Here are your arguments:\n");
        for(int i = 1; i < argc; i++)
        {
            printf("Argument #%d: %s\n", i, argv[i]);
        }
    }    
    else
    {
        printf("Too many arguments.\n");
    }
    return 0;
}
