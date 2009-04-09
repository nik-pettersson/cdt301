#/* Text: Lexical analyser 
# * Author: Niklas Pettersson, Lars Cederholm
# * Date: Tue Apr 7
# */

PROG = app.out
OBJS = scanner.o main.o
CC = gcc
LD = gcc
LDFLAGS = -o
CFLAGS = -c -Wall -o

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(PROG) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) main.o main.c
scanner.o: scanner.c
	$(CC) $(CFLAGS) scanner.o scanner.c

clean:
	rm -f $(PROG) $(OBJS) *~
