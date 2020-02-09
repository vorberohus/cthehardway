#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    struct Address *rows;
    int max_data;
    int max_rows;
    char *buffer;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void die(struct Connection *conn, const char *message);
void Database_load(struct Connection *conn);
struct Connection * Database_open(const char *filename, char mode);

size_t Address_bsize(int max_data)
{
    return sizeof(struct Address) + (max_data - sizeof(char *)) * 2;
}

char * Address_serialize(struct Address *addr, char *buffer, int max_data)
{
    memcpy(buffer, addr, sizeof(int) * 2);
    buffer += sizeof(int) * 2;
    //memcpy(buffer, &addr->set, sizeof(int));
    //buffer += sizeof(int);
    memcpy(buffer, addr->name, max_data);
    buffer += max_data;
    memcpy(buffer, addr->email, max_data);
    buffer += max_data;

    return buffer;
}

void Address_print(struct Address *addr);

char * Address_deserialize(struct Address *addr, char *buffer, int max_data)
{
    memcpy(&addr->id, buffer, sizeof(int));
    buffer += sizeof(int);
    memcpy(&addr->set, buffer, sizeof(int));
    buffer += sizeof(int);
    addr->name = buffer;
    buffer += max_data;
    addr->email = buffer;
    buffer += max_data;

    return buffer;
}

size_t Address_init(struct Address *addr, int id, int max_data)
{
    addr->id = id;
    addr->set = 0;
    addr->name = malloc(sizeof(char) * max_data);
    addr->email = malloc(sizeof(char) * max_data);

    return sizeof(addr);
}

void Address_cleanup(struct Address *addr)
{
    free(addr->name);
    free(addr->email);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

struct Connection * Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));

    if (!conn)
        die(conn, "Memory error");

    conn->db = malloc(sizeof(struct Database));

    if (!conn->db)
        die(conn, "Memory error");

    if (mode == 'c')
    {
        conn->file = fopen(filename, "w");
    }
    else
    {
        conn->file = fopen(filename, "r+");

        if (conn->file)
        {
            Database_load(conn);
        }
    }

    if (!conn->file)
        die(conn, "Failed to open file");

    return conn;
}

void Database_close(struct Connection *conn)
{
    if (conn)
    {
        if (conn->db)
        {
            free(conn->db->rows);
            free(conn->db->buffer);
            free(conn->db);         
        }

        if (conn->file)
            if (fclose(conn->file) != 0)
                die(NULL, "Failed to close file");

        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    struct Database *db = conn->db;

    int rc = fwrite(db, sizeof(struct Database), 1, conn->file);

    if (rc != 1)
        die(conn,"Failed to write database.");

    //printf("%ld\n", Address_bsize(db->max_data));
    size_t buffer_size = Address_bsize(db->max_data) * db->max_rows;
    char *buffer = malloc(buffer_size);

    char * ptr = buffer;
    for (int i = 0; i < db->max_rows; i++)
        ptr = Address_serialize(&db->rows[i], ptr, db->max_data);

    rc = fwrite(buffer, buffer_size, 1, conn->file);

    if (rc != 1)
        die(conn,"Failed to write rows.");

    rc = fflush(conn->file);

    if (rc == -1)
        die(conn,"Cannot flush database.");
}

void Database_load(struct Connection *conn)
{
    struct Database *db = conn->db;

    int rc = fread(db, sizeof(struct Database), 1, conn->file);

    if (rc != 1)
        die(conn,"Failed to load database.");

    size_t buffer_size = Address_bsize(db->max_data) * db->max_rows;
    db->buffer = malloc(buffer_size);

    long rows_size = sizeof(struct Address) * db->max_rows; 
    db->rows = malloc(rows_size);

    rc = fread(db->buffer, buffer_size, 1, conn->file);

    if (rc != 1)
        die(conn,"Failed to load rows.");

    char * ptr = db->buffer;
    for (int i = 0; i < db->max_rows; i++)
    {
        ptr = Address_deserialize(&db->rows[i], ptr, db->max_data);  
    }

    printf("Database loaded.\n");
}

void Database_create(struct Connection *conn, int max_data, int max_rows)
{
    struct Database *db = conn->db;
    db->max_data = max_data;
    db->max_rows = max_rows;

    db->rows = malloc(sizeof(struct Address) * max_rows);
    for (int i = 0; i < max_rows; i++)
        Address_init(&db->rows[i], i, max_data);
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if (addr->set)
        die(conn,"Already set, delete it first.");

    addr->set = 1;

    int max_data = conn->db->max_data;

    char *copy = strncpy(addr->name, name, max_data);
    int len = strlen(name);
    addr->name[len >= max_data ? max_data - 1 : len] = '\0';

    if (!copy)
        die(conn,"Name copy failed.");

    copy = strncpy(addr->email, email, max_data);
    len = strlen(email);
    addr->email[len >= max_data ? max_data - 1 : len] = '\0';

    if (!copy)
        die(conn,"Email copy failed.");
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if (addr->set)
        Address_print(addr);
    else
        die(conn,"ID not set.");
}

void Database_delete(struct Connection *conn, int id)
{
    conn->db->rows[id].set = 0;
}

void Database_list(struct Connection *conn)
{
    struct Database *db = conn->db;

    for (int i = 0; i < db->max_rows; i++)
    {
        struct Address *cur_addr = &db->rows[i];

        if (cur_addr->set)
            Address_print(cur_addr);
    }
}

void die(struct Connection *conn, const char *message)
{
    if(conn != NULL)
        Database_close(conn);

    if (errno)
        perror(message);
    else
        printf("Error: %s\n", message);

    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        die(NULL, "USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];

    struct Connection *conn = Database_open(filename, action);

    int id = 0;

    if (argc > 3) id = atoi(argv[3]);
//    if (id > MAX_ROWS) die(conn,"There's not that many records.");

    switch(action)
    {
        case 'c':
            if (argc != 5)
                die(conn,"Need max_data and max_rows.");
            Database_create(conn, atoi(argv[3]), atoi(argv[4]));
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4)
                die(conn,"Need an id to get.");
            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6)
                die(conn,"Need id, name and email.");

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4)
                die(conn,"Need an id to delete.");

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

        default:
            die(conn,"Invalid action: c=create, g=get, s=set, d=delete, l=list");
    }

    Database_close(conn);

    return 0;
}
