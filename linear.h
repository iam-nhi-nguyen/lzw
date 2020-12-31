// linear.h
// Contains declarations of the linear array for decode

// Written by Nhi Nguyen (nnn22)

/* -----------------------------------------------------------------------------
 * typedef struct elt Elt;
 * Fields
 *      int prefix: code of the prefix
 *      int c: last character
 *      int isPref: if it is a prefix of somethine else
 */
typedef struct elt{
    unsigned int prefix : 20;
    unsigned int c : 9;
    bool isPref : 1;
} Elt;

/* -----------------------------------------------------------------------------
 * typedef struct array Array;
 * Fields
 *      unsigned long size: size of array
 *      unsigned int nbits: maximum number of bits to represent the char
 *      unsigned int n: number of elements in array
 *      Elt *array: array that stores the elements
 */
typedef struct array{
    unsigned int nbits : 5;
    unsigned int size : 20;
    unsigned int n : 20;
    Elt *array;
} Array;

void initArray(Array *t, unsigned int nbits, int maxBits);

void arrayInsert(Array *t, unsigned int prefix, unsigned int c, int maxBits);

void arrayGrow(Array *t);

void arrayPrune(Array *t, int maxBits);

void arrayDestroy(Array t);
