// globals.h
// Contains includes, definitions, and declarations used by all files in lzw

// Written by Nhi Nguyen <nnn22>

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include "code.h"

// Print message to stderr
#define WARN(msg)	fprintf (stderr, "lzw: %s\n", msg)

// Print message to stderr and exit
#define DIE(msg)    exit (WARN(msg))

// Special code values
#define UNKNOWN			(0)
#define EMPTY 			(1)
#define PRUNE_FLAG 		(2)
#define GROW_FLAG 		(3)
#define NSPECS			(4)

// Constants
#define INITIAL_BITS 	(9)
#define ASCII_TOTAL 	(256)

// Number of bits at the beginning of the encoded file used to signify:
#define BITS_TO_SEND_MAXBITS 	(8)		// maxbits

// -----------------------------------------------------------------------------
// Options
// -----------------------------------------------------------------------------
// Fields:
//   int decode: 0 if encode, 1 if decode
//   int maxBits: the maxBits value set in -m MAXBITS
//   int prune: 1 if the -p flag is set, 0 otherwise
typedef struct options{
  	int decode;
  	int maxBits;
  	int prune;
} Options;
