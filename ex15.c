#include <stdio.h>

void print_strings(char **strings, int size);

int main(int argc, char **argv)
{
    int ids[] = { 1, 2, 3, 4 };
    char *regions[] = { "sanddesert", "windypike", "deepwater", "greenforest"};

    int ids_len = sizeof(ids)/sizeof(int);
    int regions_len = sizeof(regions)/sizeof(char*);

    print_strings(regions, ids_len);

    printf("Ages size: %d\n", ids_len);
    printf("Regions size: %d\n", regions_len);

    for(int i = 0; i < ids_len; ++i)
    {
        printf("Region: %s, id: %d, counter: %d\n", regions[i], ids[i], i);
    }

    int *ids_ptr = ids;
    char **regions_ptr = regions;

    printf("-------WITH PTRS AS ARRAYS------\n");
    for(int i = 0; i < ids_len; i++)
    {
        printf("Region: %15s, id: %3d\n", regions_ptr[i], ids_ptr[i]);
    }

    printf("-------WITH PTRS------\n");
    for(int i = 0; i < ids_len; i++)
    {
        printf("Region: %15s, id: %3d\n", *(regions_ptr + i), *(ids_ptr + i));
    }
}


void print_strings(char **strings, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", strings[i]);
    }
}

