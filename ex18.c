#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char *msg)
{
    if (errno)
        perror(msg);
    else
        printf("ERROR: %s\n", msg);

    exit(1);
}

typedef int (*fpCompare)(int a, int b);
typedef int *(*fpSort)(int * array, int count, fpCompare cmp);

void swap(int * const a, int * const b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int *bubble_sort(int *array, int count, fpCompare cmp)
{
    printf("Bubble Sort:\n");

    int *target = malloc(sizeof(int) * count);

    if (!target)
        die("Memory error!");
    
    memcpy(target, array, sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        for (int j = 1; j < count; ++j)
           if (cmp(target[j-1], target[j]) > 0)
               swap(&target[j-1], &target[j]);

    return target;
}

int *insertion_sort(int *array, int count, fpCompare cmp)
{
    printf("Insertion Sort:\n");

    int *target = malloc(sizeof(int) * count); 

    if (!target)
        die("Memory error!");

    memcpy(target, array, sizeof(int) * count);

    int k = 1;
    int j;

    for (int i = 1; i < count; ++i)
    {
        k = target[i];
        j = i - 1;

        while (j >= 0 && cmp(k, target[j]) < 0)
        {
            target[j + 1] = target[j];
            --j;
        }
        target[j + 1] = k;
    }

    return target;
}

void merge(int *array1, int count1, int *array2, int count2, fpCompare cmp)
{
    int k,j; 
    for (int i = 0; i < count2; ++i)
    {
        k = *array2;
        j = count1 - 1;

        while (j >= 0 && cmp(k, array1[j]) < 0)
        {
            array1[j + 1] = array1[j];
            --j;
        }
        array1[j + 1] = k;

        ++array2;
        ++count1;
    }
}

void merge_sort_internal(int *array, int count, fpCompare cmp)
{
    if (count > 1)
    {
        int half = count / 2;
        int rem = half + count % 2;
        merge_sort_internal(&array[0], half, cmp);
        merge_sort_internal(&array[half], rem, cmp);
        merge(&array[0], half, &array[half], rem, cmp);
    }
}

int *merge_sort(int *array, int count, fpCompare cmp)
{
    printf("Merge Sort:\n");

    int *target = malloc(sizeof(int) * count);

    if (!target)
        die("Memory error!");

    memcpy(target, array, sizeof(int) * count);

    merge_sort_internal(target, count, cmp);

    return target;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

void print_numbers(int *numbers, int count)
{
    for (int i = 0; i < count; ++i)
        printf("%3d", numbers[i]);

    printf("\n");
}

void sort_and_print(int *numbers, int count, fpSort sort, fpCompare cmp)
{
    int *sorted = sort(numbers, count, cmp);
    if (!sorted)
        die("Sort failed!");

    print_numbers(sorted, count);
    free(sorted);
}

int main(int argc, char **argv)
{
    if (argc < 2)
        printf("USAGE: ex18 n [n1 n2 n3 ...]");

    int *input_array = malloc(sizeof(int) * argc - 1);

    for (int i = 0; i < argc - 1; ++i)
        input_array[i] = atoi(argv[i + 1]);

    sort_and_print(input_array, argc - 1, merge_sort, sorted_order);
    sort_and_print(input_array, argc - 1, merge_sort, reverse_order);
    sort_and_print(input_array, argc - 1, insertion_sort, sorted_order);
    sort_and_print(input_array, argc - 1, insertion_sort, reverse_order);
    sort_and_print(input_array, argc - 1, bubble_sort, sorted_order);
    sort_and_print(input_array, argc - 1, bubble_sort, reverse_order);

    free(input_array);
}
