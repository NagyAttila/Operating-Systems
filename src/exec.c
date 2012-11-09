#include "exec.h"

void execute(Command cmd)
{
  pid_t pid;
  char** argv = cmd.pgm->pgmlist;
  if( isBuiltin(*argv) )
  {
    if ( NULL == cmd.pgm->next)
    {
      ExecBuiltin(*argv, argv);
    }
  }
  else
  {
    if(0 == (pid = Fork()))
    { 
      int in, out;
      in = STDIN_FILENO;
      out = STDOUT_FILENO;

      HandelRedirection(cmd, &in, &out);

      /* piping needed? */
      if ( cmd.pgm->next )
      {
        CreatePipeLine(cmd.pgm, in, out);
      }

      /* exec for last command in pipeline*/
      Execvp( *cmd.pgm->pgmlist, 
              cmd.pgm->pgmlist );
    }
    else
    {
      /* lsh waits forground cmd to terminate */
      if(!cmd.background)
      {
        Wait(pid);
      }
    }
  }
}

void CreatePipeLine(Pgm *p, int in, int out)
{
  char **argv;
  int pipe_fd[2];

  /* Reverse iteration */
  if (p == NULL)
    return;
  else
  {
    CreatePipeLine(p->next, in, out);

    argv = p->pgmlist;

    pipe(pipe_fd);

    if(0 == Fork())
    { 
      /* Child */
      SetStd(in,pipe_fd[0]);
      close(pipe_fd[1]);
    }
    else 
    { 
      /* Parent*/
      SetStd(out,pipe_fd[1]);
      close(pipe_fd[0]);

      Execvp(*argv, argv);
    }
  }
}

pid_t Fork()
{
  pid_t  pid;
  if ((pid = fork()) < 0)   /* fork a child process */
  {     
    perror(NULL);
    exit(1);
  }
  return pid;
}

void Execvp(const char *file, char *const argv[])
{
  if (execvp(file, argv) < 0) 
  {
    perror(file);
    exit(1);
  }
}

void SetStd(int put, int fd)
{
  close(put);
  dup(fd);
  close(fd);
}

int Wait(pid_t pid)
{
  int status;
  while ( (wait(&status) != pid) )
    ;
  return status;
}
