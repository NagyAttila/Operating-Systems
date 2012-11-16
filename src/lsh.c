#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "utils.h"
#include "parse.h"
#include "exec.h"

/*
 * Prototypes
 */

void PrintCommand(int, Command);
void PrintPgm(Pgm *);
void stripwhite(char *);
void OnInterrupt(int);

pid_t running_pid = 0;

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

  signal(SIGINT, OnInterrupt);
  
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
        pid_t new_pid = execute(cmd);
        
        if (!cmd.background) {
          running_pid = new_pid; // We only save if foreground, because we never want to interrupt background.
          waitpid(running_pid, NULL, NULL);
          // When we get here, the process with running_pid has exited
        }
        /*PrintCommand(n, cmd);*/
      }
    }

    if(line)
      free(line);
  }
  return 0;
}

void OnInterrupt(int signal) {
  if (running_pid != 0) {
    kill(running_pid, SIGTERM);
    running_pid = 0;
  }
}
