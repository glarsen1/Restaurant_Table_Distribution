#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "read_write.h"
#include "distribution.h"

int main(void)
{
    setlocale(LC_ALL, "Portuguese");

    TSTablesList* tables = NULL;
    TSClientsList* clients = NULL;

    tables = loadFileTables(tables);
    clients = loadFileClients(clients);

    printf("---- Tables loaded ----\n");
    printListTables(tables);

    printf("---- Clients loaded ----\n");
    printListClient(clients);

    groupTableAtribution(clients, tables);

    printf("---- Distribution ----\n");
    printListDistribution(clients);

    /* write updates back to CSVs (overwrites) */
    saveTablesFile(tables, FILE_TABLES);
    saveClientsFile(clients, FILE_CLIENTS);


    freeMemoryTables(tables);
    freeMemoryClients(clients);

    return 0;
}
