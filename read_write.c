/* read_write.c - loaders that skip CSV headers and trim whitespace */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "read_write.h"
#include "distribution.h"

#define MAX_LINE 512

/* helper: trim leading/trailing whitespace (modifies s in place) */
static void trim(char *s) {
    char *p = s;
    size_t len;
    /* skip leading whitespace */
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
    /* trim trailing whitespace */
    len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

/* Load tables from FILE_TABLES and insert them ordered */
TSTablesList* loadFileTables(TSTablesList* list)
{
    FILE* file = fopen(FILE_TABLES, "r");
    char buffer[MAX_LINE];

    if (file == NULL) {
        printf("\n\t\t\tError opening file %s!\n", FILE_TABLES);
        return list;
    }

    if (list != NULL) {
        printf("\n\t\t\tList already filled!\n");
        fclose(file);
        return list;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *tok;
        TSTables tmp = {0, 0, 0};

        trim(buffer);
        if (buffer[0] == '\0') continue; /* skip empty lines */

        tok = strtok(buffer, ",");
        if (tok == NULL) continue;
        trim(tok);

        /* If first token is not numeric, treat as header and skip */
        if (! (isdigit((unsigned char)tok[0]) || tok[0] == '+' || tok[0] == '-')) {
            continue; /* skip header or malformed line */
        }

        tmp.idTable = atoi(tok);

        tok = strtok(NULL, ",");
        if (tok == NULL) continue;
        trim(tok);
        tmp.nSpots = atoi(tok);

        tok = strtok(NULL, "\r\n");
        if (tok != NULL) {
            trim(tok);
            tmp.nOcupied = atoi(tok);
        }

        list = insertTableOrdered(list, &tmp);
    }

    fclose(file);
    return list;
}

void printListTables(TSTablesList* list)
{
    TSTablesList* actual = list;

    if (actual == NULL) {
        printf("\n\n##---Empty Tables List---##\n\n");
        return;
    }

    while (actual != NULL) {
        printf("\n\t\tTable ID %d, Spots %d, Occupied %d",
               actual->table.idTable,
               actual->table.nSpots,
               actual->table.nOcupied);
        actual = actual->next;
    }
    printf("\n");
}

/* Load clients from FILE_CLIENTS and insert them ordered */
TSClientsList* loadFileClients(TSClientsList* list)
{
    FILE* file = fopen(FILE_CLIENTS, "r");
    char buffer[MAX_LINE];

    if (file == NULL) {
        printf("\n\t\t\tError opening file %s!\n", FILE_CLIENTS);
        return list;
    }

    if (list != NULL) {
        printf("\n\t\t\tList already filled!\n");
        fclose(file);
        return list;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *tok;
        TSClients tmp = {0, 0, EMPTY_TABLE};

        trim(buffer);
        if (buffer[0] == '\0') continue; /* skip empty lines */

        tok = strtok(buffer, ",");
        if (tok == NULL) continue;
        trim(tok);

        /* If first token looks like header (not a single printable non-digit char), skip */
        if (strlen(tok) != 1 || isdigit((unsigned char)tok[0]) || !isprint((unsigned char)tok[0])) {
            continue; /* skip header or malformed line */
        }

        tmp.group = tok[0];

        tok = strtok(NULL, "\r\n");
        if (tok == NULL) continue;
        trim(tok);
        tmp.people = atoi(tok);

        list = insertClientsOrdered(list, &tmp);
    }

    fclose(file);
    return list;
}

void printListClient(TSClientsList* list)
{
    TSClientsList* actual = list;

    if (actual == NULL) {
        printf("\n\n##---Empty Clients List---##\n\n");
        return;
    }

    while (actual != NULL) {
        printf("\n\t\tGroup %c, People %d, AtributedTable %d",
               actual->client.group,
               actual->client.people,
               actual->client.atributedTable);
        actual = actual->next;
    }
    printf("\n");
}

void printListDistribution(TSClientsList* clients)
{
    TSClientsList* actual = clients;

    if (actual == NULL) {
        printf("\n\n##---Empty Clients List---##\n\n");
        return;
    }

    while (actual != NULL) {
        if (actual->client.atributedTable == EMPTY_TABLE) {
            printf("\nGroup %c - waiting", actual->client.group);
        } else {
            printf("\nGroup %c - table %d", actual->client.group, actual->client.atributedTable);
        }
        actual = actual->next;
    }
    printf("\n");
}

void saveTablesFile(TSTablesList* list, const char* filename)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Error opening %s for writing\n", filename);
        return;
    }

    /* write header */
    fprintf(f, "idTable,nSpots,nOcupied\n");

    TSTablesList* cur = list;
    while (cur != NULL) {
        fprintf(f, "%d,%d,%d\n",
                cur->table.idTable,
                cur->table.nSpots,
                cur->table.nOcupied);
        cur = cur->next;
    }

    fclose(f);
}

void saveClientsFile(TSClientsList* list, const char* filename)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Error opening %s for writing\n", filename);
        return;
    }

    /* write header */
    fprintf(f, "group,people,atributedTable\n");

    TSClientsList* cur = list;
    while (cur != NULL) {
        fprintf(f, "%c,%d,%d\n",
                cur->client.group,
                cur->client.people,
                cur->client.atributedTable);
        cur = cur->next;
    }

    fclose(f);
}


void freeMemoryTables(TSTablesList* list)
{
    TSTablesList* aux;
    while (list != NULL) {
        aux = list->next;
        free(list);
        list = aux;
    }
}

void freeMemoryClients(TSClientsList* list)
{
    TSClientsList* aux;
    while (list != NULL) {
        aux = list->next;
        free(list);
        list = aux;
    }
}
