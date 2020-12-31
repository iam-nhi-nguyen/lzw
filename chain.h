// chain.h
// Contains declarations of the hash with chain table for encode

// Written by Nhi Nguyen (nnn22)

/* -----------------------------------------------------------------------------
 * typedef struct node Node;
 * Fields
 *      unsigned int prefix: code of the prefix
 *      unsigned int c: last character
 *      unsigned int next: code of the next node in chain
 *      bool isPref: if it is a prefix of somethine else
 */
typedef struct node{
    unsigned int prefix : 20;
    unsigned int c : 9;
    bool isPref : 1;
    unsigned int next : 20;
} Node;

/* -----------------------------------------------------------------------------
 * typedef struct table Table;
 * Fields
 *      unsigned int prefix: code of the prefix
 *      unsigned int c: last character
 *      unsigned int next: code of the next node in chain
 *      bool isPref: if it is a prefix of somethine else
 *      unsigned int *table: the hash table that contains the code of the first
 *                              element in each chain
 *      Node *array: the array that contains all the elements
 */
typedef struct table{
    unsigned int nbits : 5;
    unsigned int nchains : 20;
    unsigned int n : 20;
    unsigned int *chain;
    Node *array;
} Table;

unsigned int hash(unsigned int prefix, unsigned int c, unsigned int nchains);

void initTable(Table *t, unsigned int nbits, int maxBits);

bool tableInsert(Table *t, unsigned int code, unsigned int prefix,
                    unsigned int c, int maxBits);

unsigned int tableGetCode(Table t, unsigned int prefix, unsigned int c);

void tableGrow(Table *t);

void tablePrune(Table *t, int maxBits);

void tableDestroy(Table t);
