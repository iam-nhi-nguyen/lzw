// encode.c
// Contains the implementation of the lzw encoder

// Written by Nhi Nguyen <nnn22>

#include "globals.h"
#include "encode.h"
#include "chain.h"

/* -----------------------------------------------------------------------------
 * encode()
 */
void encode(int maxBits, int prune){
	// Send maxBits to decode
	putBits(BITS_TO_SEND_MAXBITS, maxBits);

	// Create and initialize a table
	Table t;
    initTable(&t, INITIAL_BITS, maxBits);

	// Variables
	unsigned int C = EMPTY;	// C = EMPTY
	unsigned int K, code, h;

	// WHILE ((K = GETCHAR()) != EOF)
	while((K = getchar()) != EOF){
		code = tableGetCode(t, C, K);

		// IF THE PAIR (C,K) IS IN THE TABLE
		if(code != EMPTY){
			// issue: does not increment all the prefixes ???
			// Increment usage count of the element
            h = hash(C, K, t.nchains);
            h = t.chain[h];
            while(h){
                if(t.array[h].prefix == C && t.array[h].c == K){
                    t.array[h].isPref = true;
                    break;
                }
                h = t.array[h].next;
            }
			// SET C = CODE ASSOCIATED WITH THE PAIR (C,K) IN THE TABLE
			C = code;
		}
		else{
			// OUTPUT CODE C
			putBits(t.nbits, C);
			// INSERT THE PAIR (C,K) INTO THE TABLE
			if(tableInsert(&t, t.n, C, K, maxBits)){
				putBits(t.nbits - 1, GROW_FLAG); 		// increment flag
			}
			
			// Prune if table has reached max size
			if(t.n == (1 << maxBits) && prune){
				putBits(t.nbits, PRUNE_FLAG); // prune flag
				tablePrune(&t, maxBits);
			}
			// SET C = CODE ASSOCIATED WITH THE PAIR (EMPTY, K) IN THE TABLE
			C = tableGetCode(t, EMPTY, K);
		}
	}

	// OUTPUT CODE C (IF c != EMPTY)
	if(C != EMPTY) putBits(t.nbits, C);
	
	// Print the remaining bits still in table
	flushBits();	
	tableDestroy(t);
}
