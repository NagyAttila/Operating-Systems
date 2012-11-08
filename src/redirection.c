#include "redirection.h"

void HandelRedirection(Command cmd, int* in, int* out)
{
  if( NULL != cmd.rstdin )
  {
    *in = Open(cmd.rstdin,O_RDONLY);
  }

  if( NULL != cmd.rstdout )
  {
    *out = Open(cmd.rstdin,O_CREAT|O_WRONLY|O_TRUNC);
  }
}

int Open(const char *pathname, int flags)
{
  int fd;

  if ( (fd = open(pathname, flags)) < 0 )
  {     
    perror("ERROR:");
    exit(1);
  }

  return fd;
}
