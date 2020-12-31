// linear.c
// Contains implementation of the linear array for decode

// Written by Nhi Nguyen (nnn22)

#include "globals.h"
#include "linear.h"

/* -----------------------------------------------------------------------------
 * initElt()
 * Function
 *      Populate an element with given parameters
 * Parameters
 *      unsigned int prefix:
 *      unsigned int c:
 *      bool isPref:
 */
void initElt(Elt *e, unsigned int prefix, unsigned int c, bool isPref){
    e->prefix = prefix;
    e->c = c;
    e->isPref = isPref;
}

/* -----------------------------------------------------------------------------
 * initArray()
 * Function
 *      Populate an array with given parameters
 * Parameters
 *      unsigned int nbits:
 *      unsigned int c:
 *      bool isPref:
 */
void initArray(Array *t, unsigned int nbits, int maxBits){
    t->nbits = nbits;
    t->size = 1 << nbits;
    t->array = malloc(t->size * sizeof(Elt)); 
    for(int i = 0; i < t->size; i++){
        t->array[i].isPref = false;
    }
    t->n = NSPECS;
    // initialize the string array to contain the pair (EMPTY, K)
	for(int i = NSPECS; i < (NSPECS + ASCII_TOTAL); i++){
		arrayInsert(t, EMPTY, i-NSPECS, maxBits);
	}
}

/* -----------------------------------------------------------------------------
 * arrayInsert()
 * Function
 *      Insert into array linearly as an array
 * Parameters
 *      Array *t: pointer to the given array
 *      unsigned int prefix: code of the prefix
 *      unsigned int c: last character
 *      int maxBits: max number of bits used to represent the array
 */
void arrayInsert(Array *t, unsigned int prefix, unsigned int c, int maxBits){
    if(t->n >= (1 << maxBits)){                     // array maxed out
        return;              
    }
    if(t->n >= t->size){                            // array grow
        arrayGrow(t);
    }
    initElt(&t->array[t->n], prefix, c, false);     // populate the new element
    t->n++;
}

/* -------------------------------------------------------------------------
 * arrayGrow()
 * Function
 *      Grow a array linearly
 * Parameters
 *      Array *t: pointer to the given array
 */
void arrayGrow(Array *t){
    // update fields of t
    t->nbits++;
    t->size = t->size << 1;
    // enlarge array
    t->array = realloc(t->array, t->size * sizeof(Elt));
    for(int i = t->size >> 1; i < t->size; i++){
        t->array[i].isPref = false;
    }
}

/* -------------------------------------------------------------------------
 * arrayPrune()
 * Function
 *      Return a linear array with only elements that are prefixes of
 *      another elements
 * Parameters
 *      Array *t: pointer to the given array
 *      int maxBits: max number of bits allowed
 */

void arrayPrune(Array *t, int maxBits){
    // array whose indices are old codes and values are corresponding new codes
    unsigned int *newCodes = malloc(t->n * sizeof(unsigned int));
    memset(newCodes, UNKNOWN, t->n * sizeof(unsigned int));
    // ASCII values
    for(int i = NSPECS; i < (NSPECS + ASCII_TOTAL); i++){
        newCodes[i] = i;
    }
    int new_tn = NSPECS + ASCII_TOTAL;  // number of elements added so far

    // insert all the code that are prefix of others
    for(int code = (NSPECS + ASCII_TOTAL); code < t->n; code++){
        if(!t->array[code].isPref) continue;            // not a prefix

        newCodes[code] = new_tn;                        // assign new code
        if(newCodes[t->array[code].prefix] == UNKNOWN){ // missing prefix code
            arrayDestroy(*t); free(newCodes);
            DIE("prune: array corrupted");
        }
        // insert the element into the array
        initElt(&t->array[new_tn],newCodes[t->array[code].prefix], 
                                    t->array[code].c, false);

        t->array[code].isPref = false;
        new_tn++;
    }
    free(newCodes);

    // update fields of t
    t->n = new_tn;
    unsigned int nbits = 9;
 	while((1 << ++nbits) < t->n);
    t->nbits = nbits;

    // enlarge array
    t->size = 1 << nbits;
    t->array = realloc(t->array, t->size * sizeof(Elt));
}

/* -------------------------------------------------------------------------
 * arrayDestroy()
 * Function
 *      Free all memory associated with a given array
 * Parameters
 *      Array t: the given array
 */
void arrayDestroy(Array t){
    free(t.array);
}
