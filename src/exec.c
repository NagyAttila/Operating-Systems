#include "exec.h"

pid_t execute(Command cmd)
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
      
      InterceptWith(cmd.pgm->next);
      RunCommand(*cmd.pgm->pgmlist, cmd.pgm->pgmlist);
    } else {
      return pid;
    }
  }
  
  return 0;
}

pid_t pid = 0;
void InterceptWith(Pgm *p)
{
  if (p == NULL) {
    return;
  }
  
  int ends[2];
  pipe(ends);
  int read_end = ends[0];
  int write_end = ends[1];
  
  pid = Fork();
  if (pid == 0) {
    // Child
    
    InterceptWith(p->next);
    
    // Write to parent
    close(read_end);
    close(STDOUT_FILENO);
    dup(write_end);
    
    signal(SIGTERM, OnTerminate);
    signal(SIGINT, SIG_IGN);
    RunCommand(*p->pgmlist, p->pgmlist);
  } else {
    // Parent
    
    // Read from child
    close(write_end);
    close(STDIN_FILENO);
    dup(read_end);
    
    wait(NULL);
  }
}

void OnTerminate(int signal) {
  if (pid != 0) {
    kill(pid, SIGTERM);
    exit(1);
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
    fprintf(stderr, "Command not found.\n");
    exit(1);
  }
}

void RunCommand(const char *name, char *const argv[]) {
  if (isBuiltin(name)) {
    ExecBuiltin(name, argv);
  } else {
    Execvp(name, argv);
  }
}
