#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>
#include <readline/readline.h>

#include "parse.h"

/*
 * Name: PrintCommand
 *
 * Desc: Prints a Command structure as returned by parse on stdout.
 *
 */
extern void PrintCommand (int, Command);

/*
 * Name: PrintPgm
 *
 * Desc: Prints a list of Pgm:s
 *
 */
extern void PrintPgm (Pgm*);

/*
 * Name: stripwhite
 *
 * Desc: Strip whitespace from the start and end of STRING.
 */
extern void stripwhite (char*);

#endif
