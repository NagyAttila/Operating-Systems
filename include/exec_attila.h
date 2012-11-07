#ifndef EXEC_ATTILA_H
#define EXEC_ATTILA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parse.h"

/*
 * Pipe the commands and execute them.
 * 'in' and 'out' filedescriptors are necessary for unit testing.
 */
extern void execute(Command, int, int);

  /*
   * Pipe the commands in a reverse order, since they were 
   * saved reversed during command line parsing.
   * (The lion share of the work resides here.)
   */
extern void CreatePipeLine(Pgm*, int, int);

/*
 * Printing 'command not found' error to stdout.
 */
extern void Error_CmdNotFound(char* file);

/*
 * Wrapper for fork system call.
 */
extern pid_t Fork();

/*
 * Wrapper for execvp system call.
 */
extern void Execvp(const char *file, char *const argv[]);

/*
 * Sets the given filedescriptor to standard in or out.
 */
extern void SetStd(int put, int fd);

#endif
