#include "exec.h"

void execute(Command cmd, int in, int out)
{
  pid_t pid;

  if(0 == (pid = Fork()))
  { 
    CreatePipeLine(cmd.pgm, in, out);

    /* exec for last command in pipeline*/
    Execvp( *cmd.pgm->pgmlist, 
             cmd.pgm->pgmlist );
  }
  else
  {
    int status;
    /* lsh waits forground cmd to terminate */
    while ( !cmd.background && (wait(&status) != pid) )
      ;
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

void Error_CmdNotFound(char* file)
{
  printf("lsh: %s: command not found!\n", file);
}

pid_t Fork()
{
  pid_t  pid;
  if ((pid = fork()) < 0)   /* fork a child process */
  {     
    printf("*** ERROR: forking child process failed\n");
    exit(1);
  }
  return pid;
}

void Execvp(const char *file, char *const argv[])
{
  if (execvp(file, argv) < 0) 
  {
    Error_CmdNotFound(argv[0]);
    exit(1);
  }
}

void SetStd(int put, int fd)
{
  close(put);
  dup(fd);
  close(fd);
}

