# Written by Nhi Nguyen <nnn22>

# Macro definition
CC     	= gcc
CFLAGS 	= -g3 -std=c99 -pedantic -Wall
HWK 	= /c/cs323/Hwk4

# Rules
all: encode decode

encode: lzw.c globals.h encode.o decode.o chain.o linear.o code.o 
	${CC} ${CFLAGS} -o $@ $^

decode: encode
	ln -f encode decode

encode.o: encode.h encode.c

decode.o: decode.h decode.c

chain.o: chain.h chain.c

linear.o: linear.h linear.c

code.o: code.h code.c

clean:
	$(RM) encode decode *.o
