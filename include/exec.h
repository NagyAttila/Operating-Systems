#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "parse.h"       /* Command */
#include "redirection.h" /* HandleRedirection() */
#include "builtin.h"     /* SetStd() */

/*
 * Pipe the commands and execute them.
 */
extern void execute(Command);

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
 * Wrapper for wait system call.
 */
extern int Wait(pid_t pid);

#endif
