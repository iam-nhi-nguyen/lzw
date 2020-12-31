// chain.c
// Contains implementation of the hash with chain table for encode

// Written by Nhi Nguyen (nnn22)

#include "globals.h"
#include "chain.h"

/* -----------------------------------------------------------------------------
 * hash()
 * Function
 *      Compute the hashed index of a pair (PREF, CHAR) in a table
 * Parameters
 *      unsigned int prefix: the prefix code
 *      unsigned int c: the last character
 *      unsigned int nchains: the nchains of the hash table
 */
unsigned int hash(unsigned int prefix, unsigned int c, unsigned int nchains){
    return (((unsigned int)(prefix) << 8) | (unsigned)(c)) % nchains;
}

/* -----------------------------------------------------------------------------
 * initNode()
 * Function
 *      Populate a node with given parameters
 * Parameters
 *      unsigned int prefix:
 *      unsigned int c:
 *      bool isPref:
 *      unsigned int next:
 */
void initNode(Node *e, unsigned int prefix, unsigned int c, bool isPref, 
                        unsigned int next){
    e->prefix = prefix;
    e->c = c;
    e->isPref = isPref;
    e->next = next;
}

/* -----------------------------------------------------------------------------
 * initTable()
 * Function
 *      Populate a table with given parameters
 * Parameters
 *      unsigned int nbits:
 *      unsigned int c:
 *      bool isPref:
 */
void initTable(Table *t, unsigned int nbits, int maxBits){
    t->nbits = nbits;
    t->nchains = (1 << (nbits-3)) - 1;
    t->chain = calloc(t->nchains, sizeof(unsigned int));
    t->array = malloc((1 << nbits) * sizeof(Node));
    for(int i = 0; i < (1 << nbits); i++){
        t->array[i].next = 0;
    }
    t->n = NSPECS;
    // initialize the string array to contain the pair (EMPTY, K)
	for(int i = NSPECS; i < (NSPECS + ASCII_TOTAL); i++){
		tableInsert(t, i, EMPTY, i-NSPECS, maxBits);
	}
}

/* -----------------------------------------------------------------------------
 * tableInsert()
 * Function
 *      Insert a new triple into table as hash with chain
 *      Return 1 if the table grows; 0 otherwise
 * Parameters
 *      Table *t: pointer to the table
 *      unsigned int code: numerical code
 *      unsigned int prefix: code of the prefix
 *      int c: last character
 *      int maxBits: max number of bits used to represent the table
 */
bool tableInsert(Table *t, unsigned int code, unsigned int prefix,
                    unsigned int c, int maxBits){
    if(t->n >= (1 << maxBits)){                 // table maxed out
        return 0;          
    }
    bool grow = false;
    if(t->n >= (1 << t->nbits)){                // table grow
        tableGrow(t);
        grow = true;
    }
    unsigned int h = hash(prefix, c, t->nchains);  // populate the new node
    initNode(&t->array[code], prefix, c, false, t->chain[h]);
    t->chain[h] = code;                         // update the head of the chain
    t->n++;                      
    return grow;
}

/* -----------------------------------------------------------------------------
 * tableGetCode()
 * Function
 *      Returns the code of the element in a given table
 *      with given prefix and final character
 * Parameters
 *      Table *t: pointer to the table
 *      unsigned int prefix: code of the prefix
 *      int c: last character
 */
unsigned int tableGetCode(Table t, unsigned int prefix, unsigned int c){
    unsigned int h = hash(prefix, c, t.nchains);
    for(int code = t.chain[h]; code; code = t.array[code].next){
        if(t.array[code].prefix == prefix && t.array[code].c == c){
            return code;
        }
    }
    return EMPTY;
}

/* -----------------------------------------------------------------------------
 * tableGrow()
 * Function
 *      Reharsh all element from one table to another using hash with chain
 * Parameters
 *      Table *t: pointer to the table
 */
void tableGrow(Table *t){
    // update fields of t
    t->nbits++;
    t->nchains = (1 << (t->nbits-3)) - 1;
    // enlarge array
    t->array = realloc(t->array, (1 << t->nbits) * sizeof(Node));
    for(int i = t->n; i < (1 << t->nbits); i++){
        t->array[i].next = 0;
    }
    // update a new chain table
    free(t->chain);
    t->chain = calloc(t->nchains, sizeof(unsigned int));
    unsigned int h;
    for(int code = NSPECS; code < t->n; code++){
        h = hash(t->array[code].prefix, t->array[code].c, t->nchains);
        t->array[code].next = t->chain[h];
        t->chain[h] = code;
    }
}

/* -----------------------------------------------------------------------------
 * tablePrune()
 * Function
 *      Return a new hash table with only nodes that are prefixes of
 *      another node
 * Parameters
 *      Table *t: pointer to the given table
 *      int maxBits: max number of bits allowed
 */
void tablePrune(Table *t, int maxBits){
    // next field in array[i] temporary represents the values of new codes
    // initialize the values
    for(int i = NSPECS; i < (NSPECS + ASCII_TOTAL); i++){
        t->array[i].next = UNKNOWN;
    }
    for(int i = NSPECS; i < (NSPECS + ASCII_TOTAL); i++){
        t->array[i].next = i;
    }

    int new_tn = NSPECS + ASCII_TOTAL;  // number of elements counted so far

    for(int code = (NSPECS + ASCII_TOTAL); code < t->n; code++){
        if(!t->array[code].isPref) continue;            // not a prefix

        t->array[code].next = new_tn;                   // assign new code

        // missing prefix code
        if(t->array[t->array[code].prefix].next == UNKNOWN){
            tableDestroy(*t);
            DIE("prune: table corrupted");
        }
        // store the new code for prefix
        t->array[code].prefix = t->array[t->array[code].prefix].next;
        new_tn++;
    }

    // update the elements at the correct new codes
    for(int code = (NSPECS + ASCII_TOTAL); code < t->n; code++){
        if(!t->array[code].isPref) continue;

        initNode(&t->array[t->array[code].next], t->array[code].prefix,
                                    t->array[code].c, false, 0);
    }

    // update fields of t
    t->n = new_tn;
    unsigned int nbits = 9;
 	while((1 << ++nbits) < t->n);
    t->nbits = nbits;
    
    // enlarge array
    t->nchains = (1 << (nbits-3)) - 1;
    t->array = realloc(t->array, (1 << t->nbits) * sizeof(Node));

    // update a new chain table
    free(t->chain);
    t->chain = calloc(t->nchains, sizeof(unsigned int));
    unsigned int h;
    for(int code = NSPECS; code < t->n; code++){
        h = hash(t->array[code].prefix, t->array[code].c, t->nchains);
        t->array[code].next = t->chain[h];
        t->chain[h] = code;
    }
}

/* -----------------------------------------------------------------------------
 * tableDestroy()
 * Function
 *      Free all memory associated with a given table
 * Parameters
 *      Table t: the given table
 */
void tableDestroy(Table t){
    free(t.chain); free(t.array);
}
