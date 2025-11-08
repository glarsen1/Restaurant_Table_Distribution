# Restaurant Table Distribution System

This program simulates the automatic assignment of restaurant tables to client groups based on the number of people in each group.

It loads data from CSV files, distributes clients to tables according to capacity, displays the results, and can optionally save the updated data back to CSV.

---

## Project Structure

```
.
├── main.c
├── read_write.c
├── read_write.h
├── distribution.c
├── distribution.h
├── tables.csv
├── clients.csv
└── README.md
```

---

## Compilation

Use **GCC** to compile all source files:

```bash
gcc main.c read_write.c distribution.c -o distro -Wall -Wextra
```

If you want a simpler build process, you can also create a `Makefile` and just run `make`.

---

## Running the Program

After compilation, run:

```bash
./distro
```

### Expected Output Example

```
---- Tables loaded ----
        Table ID 1, Spots 2, Occupied 0
        Table ID 2, Spots 2, Occupied 0
        Table ID 3, Spots 4, Occupied 0
        Table ID 4, Spots 4, Occupied 0
        Table ID 5, Spots 6, Occupied 0
        Table ID 6, Spots 6, Occupied 0
        Table ID 7, Spots 8, Occupied 0
        Table ID 8, Spots 8, Occupied 0

---- Clients loaded ----
        Group A, People 2, AtributedTable 0
        Group B, People 3, AtributedTable 0
        ...

---- Distribution ----
Group A - table 1
Group B - table 3
Group C - table 5
Group D - table 7
Group E - table 4
Group F - table 2
Group G - table 8
Group H - waiting
```

---

## Input Files

### `tables.csv`

Defines the restaurant tables.

|     Column | Description                                             |
| ---------: | ------------------------------------------------------- |
|  `idTable` | Table ID number                                         |
|   `nSpots` | Number of seats at the table                            |
| `nOcupied` | Number of people currently seated (usually starts at 0) |

**Example:**

```csv
idTable,nSpots,nOcupied
1,2,0
2,2,0
3,4,0
4,4,0
5,6,0
6,6,0
7,8,0
8,8,0
```

---

### `clients.csv`

Defines each client group.

|   Column | Description                   |
| -------: | ----------------------------- |
|  `group` | Group name (a single letter)  |
| `people` | Number of people in the group |

**Example:**

```csv
group,people
A,2
B,3
C,5
D,8
E,4
F,2
G,7
H,9
```

---

## Distribution Rules

* Groups are seated at the smallest available table that fits their size:

  * ≤2 people → table for 2
  * ≤4 people → table for 4
  * ≤6 people → table for 6
  * ≤8 people → table for 8
* Groups larger than 8 are **not seated** and remain *waiting*.
* Tables are assigned in ascending order of ID.
* Each table can only seat one group at a time.

---

## Saving Updated Data

After distribution, you can save the results to CSV files.

If your `main.c` includes:

```c
saveTablesFile(tables, FILE_TABLES);
saveClientsFile(clients, FILE_CLIENTS);
```

the program will overwrite the original CSVs with updated data.

You can also save to new files to preserve the originals:

```c
saveTablesFile(tables, "tables_out.csv");
saveClientsFile(clients, "clients_out.csv");
```

---

## Memory Management

All dynamically allocated lists are properly freed at the end of execution using:

```c
freeMemoryTables(tables);
freeMemoryClients(clients);
```

---

## Troubleshooting

* **Error: “No such file or directory”** → Make sure you run the program in the same folder as `tables.csv` and `clients.csv`.
* **Weird table or group with ID 0 or letter g** → Your old version of the loader was reading the CSV header. Use the fixed `read_write.c` provided.
* **Group still waiting** → The group is larger than any available table.

---

## Author

Developed by **Gonçalo Larsen**
Universidade Aberta — *Laboratório de Programação (C A1S2)*
2025

---

## License

This project is for educational purposes and can be freely used and modified for lear
