.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
#BIN=01byteorder 02addr echosrv echocli echosrv2 p2psrv p2pcli echosrv3 echocli3
BIN=echosrv4 echocli4
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)

