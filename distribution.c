#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "read_write.h"
#include "distribution.h"

/* Insert table nodes ordered by idTable (ascending) */
TSTablesList* insertTableOrdered(TSTablesList* ordered, TSTables* newTable)
{
    TSTablesList* element = (TSTablesList*)malloc(sizeof(TSTablesList));
    if (element == NULL) {
        printf("Allocation error\n");
        return ordered;
    }

    element->table = *newTable;
    element->next = NULL;

    /* insert at head or in middle */
    if (ordered == NULL || ordered->table.idTable >= newTable->idTable) {
        element->next = ordered;
        return element;
    }

    TSTablesList* prev = ordered;
    TSTablesList* cur = ordered->next;
    while (cur != NULL && cur->table.idTable < newTable->idTable) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = element;
    element->next = cur;
    return ordered;
}

/* Insert clients ordered by group char ascending */
TSClientsList* insertClientsOrdered(TSClientsList* ordered, TSClients* newClient)
{
    TSClientsList* element = (TSClientsList*)malloc(sizeof(TSClientsList));
    if (element == NULL) {
        printf("Allocation Error\n");
        return ordered;
    }

    element->client = *newClient;
    element->client.atributedTable = EMPTY_TABLE;
    element->next = NULL;

    if (ordered == NULL || ordered->client.group >= newClient->group) {
        element->next = ordered;
        return element;
    }

    TSClientsList* prev = ordered;
    TSClientsList* cur = ordered->next;
    while (cur != NULL && cur->client.group < newClient->group) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = element;
    element->next = cur;
    return ordered;
}

/* Determine required table topology for given people */
int ClientTopology(int people)
{
    if (people <= TABLE_2) return TABLE_2;
    if (people <= TABLE_4) return TABLE_4;
    if (people <= TABLE_6) return TABLE_6;
    if (people <= TABLE_8) return TABLE_8;
    return EMPTY_TABLE;
}

/* Find a suitable empty table with size <= topology and >= people */
int SeatGroup(TSTablesList* tables, int topology, int people)
{
    TSTablesList* cur = tables;
    while (cur != NULL) {
        if (cur->table.nOcupied == 0 && cur->table.nSpots >= people && cur->table.nSpots <= topology) {
            cur->table.nOcupied = people;
            return cur->table.idTable;
        }
        cur = cur->next;
    }
    return EMPTY_TABLE;
}

/* Use the naming from main: groupTableAtribution */
void groupTableAtribution(TSClientsList* clients, TSTablesList* tables)
{
    TSClientsList* curClient = clients;

    if (curClient == NULL || tables == NULL) {
        printf("\n\n##---Empty List(s)---##\n\n");
        return;
    }

    while (curClient != NULL) {
        int topology = ClientTopology(curClient->client.people);
        if (topology != EMPTY_TABLE) {
            curClient->client.atributedTable = SeatGroup(tables, topology, curClient->client.people);
        } else {
            printf("Group %c has invalid number of people (%d)\n", curClient->client.group, curClient->client.people);
            curClient->client.atributedTable = EMPTY_TABLE;
        }
        curClient = curClient->next;
    }
}
