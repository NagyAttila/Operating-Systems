#
# Sample Makefile for lsh
# lab1 in Operating System course
#
BIN=	lsh

SRC_DIR=../src
INC_DIR=../include

SRCS=	$(SRC_DIR)/parse.c \
			$(SRC_DIR)/lsh.c

OBJS= ${SRCS:.c=.o}

CC=	gcc
CFLAGS= -ggdb -Wall -pedantic
LIBS= -lreadline 
#LIBS= -lreadline -ltermcap
INCLUDES= -I$(INC_DIR)

.SUFFIXES: .c .o

%.o: %.c 
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $@ $<

all:	$(BIN)

lsh:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(BIN)

clean:
	-rm -f $(OBJS) lsh
