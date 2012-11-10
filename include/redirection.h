#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <fcntl.h>      /* open() */
#include <stdio.h>      /* perror() */
#include <stdlib.h>     /* error() */
#include <unistd.h>     /* STDIN_FILENO */

#include "parse.h"

extern void HandelRedirection(Command, int*, int*);

extern int Open(const char*, int);

/*
 * Sets the given filedescriptor to standard in or out.
 */
extern void SetStd(int put, int fd);

#endif
