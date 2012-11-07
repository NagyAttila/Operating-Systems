#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "utils.h"
#include "parse.h"
#include "exec_attila.h"

/*
 * Prototypes
 */

void PrintCommand(int, Command);
void PrintPgm(Pgm *);
void stripwhite(char *);

/* When non-zero, this global means the user is done using this program. */
int done = 0;

/*
 * Name: main
 *
 * Desc: Gets the ball rolling...
 *
 */
int main(void)
{
  Command cmd;
  int n;

  while (! done) {

    char *line;

    line = readline("@ ");

    if (!line)
    {
      /* Encountered EOF at top level */
      done = 1;
    }
    else
    {
      /*
       * Remove leading and trailing whitespace from the line
       * Then, if there is anything left, add it to the history list
       * and execute it.
       */
      stripwhite(line);

      if(*line)
      {
        add_history(line);
        n = parse(line, &cmd);
        execute(cmd, 0, 1);
        PrintCommand(n, cmd);
      }
    }

    if(line)
      free(line);
  }
  return 0;
}

