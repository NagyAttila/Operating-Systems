#include "builtin.h"

int isBuiltin(char* cmd)
{
  return
    (0 == strcmp(cmd,"cd") || 
     0 == strcmp(cmd,"exit"));
}

void ExecBuiltin(const char *cmd, char *const argv[])
{
  if( 0 == strcmp(cmd,"cd") )
  {
    if (argv[1] == NULL) {
      chdir(getenv("HOME"));
    } else if (argv[2] == NULL) {
      int result = chdir(argv[1]);
      if (result != 0) {
        printf("No such directory: %s\n", argv[1]);
      }
    } else {
      puts("Too many arguments.");
    }
  }
  else if( 0 == strcmp(cmd,"exit") )
  {
    exit(errno);
  }
}

