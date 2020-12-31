This is an implementation of the LZW compression scheme, written for CPSC 323 (FA20) System Programming and Computer Organization.

It creates an encoder and decoder used to compress and decompress files.

      % encode [-m MAXBITS] [-p]
       
      % decode
      
> `encode` reads a stream of characters from the standard input, compresses it using the Lempel-Ziv-Welch algorithm, and writes the stream of codes to the standard output as a stream of bits packed into 8-bit bytes. `decode` reads from the standard input a byte stream written by encode, decompresses the stream of codes, and writes the stream of characters to the standard output.

> `encode` writes each code using the smallest number of bits required to specify valid codes at the time (e.g., 9 bits when there are 512 valid codes, but 10 bits once the next code is assigned), up to a maximum of 12 bits (or `MAXBITS` if the `-m MAXBITS` option is specified).

> encode begins with a string table that assigns a code to every one-character string and assigns new codes to other strings that it finds during the greedy
parse.  Normally it stops assigning codes when the table is full (i.e., there are no unassigned codes).  But with the `-p` (pruning) option, it instead creates
a new string table that contains every one-character string and every string that is a prefix of another code in the old table.
