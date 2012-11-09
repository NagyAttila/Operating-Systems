#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <fcntl.h>      /* open() */
#include <stdio.h>      /* perror() */
#include <stdlib.h>     /* error() */
#include <unistd.h>     /* STDIN_FILENO */

#include "parse.h"

void HandelRedirection(Command, int*, int*);

int Open(const char*, int);

#endif
