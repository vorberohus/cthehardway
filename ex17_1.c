#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DATA 512

void multiple_reads(FILE *file);
void single_read(FILE *file);
double read_file_with(const char *, void (*)(FILE *));

struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    int max_data;
    int max_rows;
    struct Address *rows;
};

int main(int argc, char *argv[])
{
    double time1 = read_file_with(argv[1], multiple_reads);
    double time2 = read_file_with(argv[1], multiple_reads);
    double time3 = read_file_with(argv[1], single_read);

    printf("%f\n%f\n%f\n", time1, time2, time3); 
    return 0;
}

double read_file_with(const char *filename, void (*delegate)(FILE *))
{
    clock_t t = clock();

    FILE *db_file = fopen(filename, "r");
    if(db_file == NULL) 
        exit(1);

    delegate(db_file);
    fclose(db_file);

    t = clock() - t;
    double time_spent = ((double)t) / CLOCKS_PER_SEC;

    return time_spent;
}

void single_read(FILE *file)
{
    struct Database *db = malloc(sizeof(struct Database));
    fread(db, sizeof(struct Database), 1, file);

    db->rows = malloc(sizeof(struct Address) * db->max_rows);
    fread(db->rows, sizeof(struct Address), db->max_rows, file);

    free(db->rows);
    free(db);
    return;
}

void multiple_reads(FILE *file)
{
    struct Database *db = malloc(sizeof(struct Database));
    fread(db, sizeof(struct Database), 1, file);

    db->rows = malloc(sizeof(struct Address) * db->max_rows);

    for (int i = 0; i < db->max_rows; ++i)
        fread(&db->rows[i], sizeof(struct Address), 1, file);

    free(db->rows);
    free(db);
    return;
}
