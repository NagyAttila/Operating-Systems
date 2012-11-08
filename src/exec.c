#include "exec.h"

void execute(Command cmd, int in, int out)
{
  pid_t pid;

  if(0 == (pid = Fork()))
  { 
    /* REMARK: This function screw up 'in' and 'out' file descriptors,
     * either we give 2 more descriptors or only unit test the 
     * CreatePipeLine functions
     */
    HandelRedirection(cmd, &in, &out);

    /* piping needed? */
    if ( cmd.pgm->next )
    {
      CreatePipeLine(cmd.pgm, in, out);
    }
    else
    {
      /* REMARK: Can be moved to HandleRedirection if 
       * 'int in' and 'int out' are not needed
       */
      if( NULL != cmd.rstdin )
      {
        SetStd(0,in);
      }
      if( NULL != cmd.rstdout )
      {
        SetStd(1,out);
      }
    }

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

