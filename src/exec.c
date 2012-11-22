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
      // Redirect input from file if applicable.
      if (cmd.rstdin != NULL) {
        int in_file = Open(cmd.rstdin, O_RDONLY);
        close(STDIN_FILENO);
        dup(in_file);
      }
      
      // Redirect output to file if applicable.
      if (cmd.rstdout != NULL) {
        int out_file = Open(cmd.rstdout, O_CREAT|O_WRONLY|O_TRUNC);
        close(STDOUT_FILENO);
        dup(out_file);
      }
      
      // Ignore Ctrl-C but terminate children when asked to.
      // This is done inside InterceptWith but not for the first process in
      // the pipeline, so we do it here as well.
      signal(SIGINT, SIG_IGN);
      signal(SIGTERM, OnTerminate);
      
      InterceptWith(cmd.pgm->next);
      RunCommand(*cmd.pgm->pgmlist, cmd.pgm->pgmlist);
    } else {
      return pid;
    }
  }
  
  return 0;
}

// Holds the pid of the child process if there is one, allowing us to terminate
// it later on if Ctrl-C is pressed.
pid_t pid = 0;

// Execute the given command, "stealing" the input used by the current process 
// and sending the output to the current process's input instead.
// Recursive method of implementing pipes. Note that it does nothing if called 
// with NULL.
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
    
    // Ignore Ctrl-C but terminate children when asked to.
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, OnTerminate);
    
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

// When the user presses Ctrl-C, we get this signal from the parent.
// Terminate a child process if there is one, then terminate this process.
void OnTerminate(int signal) {
  if (pid != 0) {
    // The child should in turn kill its own child if there is one.
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

// Run either a built-in command or a file.
void RunCommand(const char *name, char *const argv[]) {
  if (isBuiltin(name)) {
    ExecBuiltin(name, argv);
  } else {
    Execvp(name, argv);
  }
}
