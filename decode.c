// decode.c
// Contains the implementation of the lzw decoder

// Written by Nhi Nguyen <nnn22>

#include "globals.h"
#include "decode.h"
#include "linear.h"

/* -----------------------------------------------------------------------------
 * recuPutchar()
 * Function
 *		Recursively prints characters tracing back all the prefixes.
 * Parameters
 *		Elt *array: array of (PREF, CHAR)
 *		unsigned int C: code of the final pair (PREF, CHAR)
 */
void recuPutchar(Elt *array, unsigned int C){
	if(array[C].prefix != EMPTY){	// recursively print prefix then character
		recuPutchar(array, array[C].prefix);
		putchar(array[C].c);
	}
	else putchar(array[C].c);		// earliest character
}

/* -----------------------------------------------------------------------------
 * decode()
 */
void decode(){
	// Get maxBits from encode
	int maxBits = getBits(BITS_TO_SEND_MAXBITS);
	if(maxBits <= 8 || maxBits > 20) DIE("input file corrupted");

	// Create and initialize array
	Array t;
    initArray(&t, INITIAL_BITS, maxBits);
	
	// Variables
	unsigned int oldC = EMPTY;	// OLDC = EMPTY
	unsigned int newC, C;
	unsigned int currC;			// previous code - changed when tracing stack

	while((newC = C = getBits(t.nbits)) != EOF){
		if((C < 0) || (C > t.n)){				// corrupted code
			arrayDestroy(t);
			DIE("byte stream corrupted");
		}

		// set isPref of previous element and all prefixes of element to True
		currC = oldC;
		while(currC != EMPTY){
			if(t.array[currC].isPref) break;	// all prefixes have been set
			t.array[currC].isPref = true;
			currC = t.array[currC].prefix;
		}

		if(C == GROW_FLAG){						// increment flag
			arrayGrow(&t);
			continue;
		}
		if(C == PRUNE_FLAG){					// prune flag
			arrayPrune(&t, maxBits);
			oldC = EMPTY;						// update previous code
			continue;
		}

		if(oldC != EMPTY){
			if(t.n < (1 << maxBits)){			//array not full
				if(C == t.n) C = oldC; 			// issue: when does this happen?
				while(t.array[C].prefix != EMPTY){
					C = t.array[C].prefix;
				}
				arrayInsert(&t, oldC, t.array[C].c, maxBits);
			}
		}
		recuPutchar(t.array, newC);
		oldC = newC;
	}
	arrayDestroy(t);
}