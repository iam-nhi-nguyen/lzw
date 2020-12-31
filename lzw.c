// lzw.c
// Parses arguments and hands off execution to encode and decode

// Written by Nhi Nguyen <nnn22>

#define _GNU_SOURCE
#include "globals.h"
#include "encode.h"
#include "decode.h"

/* -----------------------------------------------------------------------------
 * parse()
 * Function
 *     Parses and validates the command line arguments, which can be
 *         % encode [-m MAXBITS] [-p]
 *         % decode
 *     MAXBITS should be a sequence of decimal digits representable by long int
 *	   The parsed options are stored in a struct Options
 * Parameters
 *     int argc: number of command line arguments
 *     char* argv[]: array of strings of command line arguments
 *     Options *opt: a pointer to an Options struct to store the parsed options
 */
void parse(int argc, char **argv, Options *opt){
	if(!argc) DIE("no command line argument");

	// program calls encode
	if(!strcmp(argv[0], "./encode")){
		long mBits;	// the maxbits value to send to encode
		char *end;	// check for errors in strtol
		for(int i = 1; i < argc; i++){
			// -m MAXBITS
			if(!strcmp(argv[i], "-m")){
				// check the next argument for MAXBITS
				if(argc > ++i){
					mBits = strtol(argv[i], &end, 10);
					if((errno == ERANGE) || ((*end) != '\0') || (mBits <= 0)){
						DIE("invalid MAXBITS for -m flag");
					}
				}
				else DIE("missing MAXBITS for -m flag");
				// store the value of MAXBITS
				if(mBits <= 8 || mBits >20)	opt->maxBits = 12;
				else						opt->maxBits = (int)mBits;
			}
			// -p
			else if(!strcmp(argv[i], "-p")) opt->prune = 1;
			// invalid flag
			else DIE("invalid option");
		}
	}
	// program calls decode
	else if(!strcmp(argv[0], "./decode")){
		// decode should have no more argument
		if(argc > 1) DIE("too many arguments for decode");
		opt->decode = 1;
	}
	// program calls neither
	else DIE("argument should call encode or decode");
}

/* -----------------------------------------------------------------------------
 * main()
 * Function
 *     Highest level parsing arguments and handing off execution to en/decode
 */
int main(int argc, char **argv){
	// reduce the size of the buffers and allocate them statically
	static char bin[64], bout[64];
    setvbuf (stdin,  bin,  _IOFBF, 64);
    setvbuf (stdout, bout, _IOFBF, 64);
	
	// parse arguments
  	Options opt = {.decode = 0, .maxBits = 12, .prune = 0};
  	parse(argc, argv, &opt);
	
	// hand off execution to appropriate function
  	if(opt.decode) 	decode();
  	else			encode(opt.maxBits, opt.prune);
	
	// exit instead of return
	exit(0);
}
