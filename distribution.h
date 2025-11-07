#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include "read_write.h"

/* insert functions used by file loaders */
TSTablesList* insertTableOrdered(TSTablesList* ordered, TSTables* newTable);
TSClientsList* insertClientsOrdered(TSClientsList* ordered, TSClients* newClient);

/* distribution helpers */
int ClientTopology(int people);
int SeatGroup(TSTablesList* tables, int topology, int people);

/* main distribution function (name used by your main) */
void groupTableAtribution(TSClientsList* clients, TSTablesList* tables);

#endif /* DISTRIBUTION_H */
