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
    chdir(argv[1]);
  }
  else if( 0 == strcmp(cmd,"exit") )
  {
    exit(errno);
  }
}

