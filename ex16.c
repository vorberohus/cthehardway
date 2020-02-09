#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct Person
{
    char* name;
    int age;
    int height;
    int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight)
{
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != NULL);

    who->name = strdup(name);
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
}

void Person_destroy(struct Person *who)
{
    assert(who != NULL);

    free(who->name);
    free(who);
}

void Person_print(struct Person *who)
{
    assert(who != NULL);

    printf("Name: %s\n", who->name);
    printf("\t%-8s: %4d\n", "Age", who->age);
    printf("\t%-8s: %4d\n", "Height", who->height);
    printf("\t%-8s: %4d\n", "Weight", who->weight);
}

int main(int argc, char **argv)
{
    struct Person *joe = Person_create("Joe Tribiani", 25, 180, 87);
    struct Person *chandler = Person_create("Chandler Bing", 26, 185, 75);

    printf("%s is at mem address %p.\n", joe->name, joe);
    Person_print(joe);

    printf("%s is at mem address %p.\n", chandler->name, chandler);
    Person_print(chandler);

    Person_destroy(joe);
    Person_destroy(chandler);

    return 0;
}
