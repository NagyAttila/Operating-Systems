#include "redirection.h"

int Open(const char *pathname, int flags)
{
  int fd;

  if ( (fd = open(pathname, flags, S_IRUSR|S_IWUSR)) < 0 )
  {     
    perror(pathname);
    exit(1);
  }

  return fd;
}

