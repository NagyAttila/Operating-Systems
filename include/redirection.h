#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <fcntl.h>      /* open() */
#include <stdio.h>      /* perror() */
#include <stdlib.h>     /* error() */

extern int Open(const char*, int);

#endif
