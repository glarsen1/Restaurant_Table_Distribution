#ifndef READ_WRITE_H
#define READ_WRITE_H

#define FILE_TABLES "tables.csv"
#define FILE_CLIENTS "clients.csv"

#define EMPTY_TABLE 0

#define TABLE_2 2
#define TABLE_4 4
#define TABLE_6 6
#define TABLE_8 8

/* Table struct */
typedef struct STables {
    int idTable;
    int nSpots;
    int nOcupied;
} TSTables;

/* Linked list node for tables */
typedef struct STablesList {
    TSTables table;
    struct STablesList* next;
} TSTablesList;

/* Client struct */
typedef struct SClients {
    char group;
    int people;
    int atributedTable;
} TSClients;

/* Linked list node for clients */
typedef struct SClientsList {
    TSClients client;
    struct SClientsList* next;
} TSClientsList;

/* read/write functions */
TSTablesList* loadFileTables(TSTablesList* list);
void printListTables(TSTablesList* list);

TSClientsList* loadFileClients(TSClientsList* list);
void printListClient(TSClientsList* list);
void printListDistribution(TSClientsList* clients);

/* Save updated lists back to CSV files */
void saveTablesFile(TSTablesList* list, const char* filename);
void saveClientsFile(TSClientsList* list, const char* filename);


void freeMemoryTables(TSTablesList* list);
void freeMemoryClients(TSClientsList* list);

#endif /* READ_WRITE_H */
