#include "redirection.h"

void HandelRedirection(Command cmd, int* in, int* out)
{
  if( NULL != cmd.rstdin )
  {
    *in = Open(cmd.rstdin,O_RDONLY);
    SetStd(STDIN_FILENO, *in);
  }

  if( NULL != cmd.rstdout )
  {
    *out = Open(cmd.rstdout,O_CREAT|O_WRONLY|O_TRUNC);
    SetStd(STDOUT_FILENO, *out);
  }
}

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

void SetStd(int put, int fd)
{
  close(put);
  dup(fd);
  close(fd);
}

