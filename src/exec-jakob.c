#include "exec.h"

void execute(Command cmd)
{
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
    pid_t pid = Fork();
    if (pid == 0) {
      if (cmd.rstdin != NULL) {
        int in_file = Open(cmd.rstdin, O_RDONLY);
        close(STDIN_FILENO);
        dup(in_file);
      }
      
      if (cmd.rstdout != NULL) {
        int out_file = Open(cmd.rstdout, O_CREAT|O_WRONLY|O_TRUNC);
        close(STDOUT_FILENO);
        dup(out_file);
      }
      
      InterceptWith(cmd.pgm);
      execvp(*(cmd.pgm->pgmlist), cmd.pgm->pgmlist);
    } else {
      wait(NULL);
    }
  }
}

void InterceptWith(Pgm *p)
{
  if (p == NULL) {
    return;
  }
  
  int ends[2];
  pipe(ends);
  int read_end = ends[0];
  int write_end = ends[1];
  
  pid_t pid = Fork();
  if (pid == 0) {
    // Child
    
    InterceptWith(p->next);
    
    // Write to parent
    close(read_end);
    close(STDOUT_FILENO);
    dup(write_end);
    
    execvp(*(p->pgmlist), p->pgmlist);
  } else {
    // Parent
    
    // Read from child
    close(write_end);
    close(STDIN_FILENO);
    dup(read_end);
    
    wait(NULL);
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

int Wait(pid_t pid)
{
  int status;
  while ( (wait(&status) != pid) )
    ;
  return status;
}
