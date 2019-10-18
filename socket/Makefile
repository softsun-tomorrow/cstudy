.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
#BIN=01byteorder 02addr 
#BIN=echosrv echocli echosrv2 echocli2
#BIN=p2psrv p2pcli 
#BIN=echosrv3 echocli3
#BIN=echosrv4 echocli4
#BIN=echosrv5 echocli5
#BIN=echosrv6 echocli6
BIN=echosrv7 echocli7
#BIN=getiplist

all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)

