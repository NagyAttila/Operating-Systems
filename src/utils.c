#include "utils.h"

void
PrintCommand (int n, Command cmd)
{
  printf("Parse returned %d:\n", n);
  printf("   stdin : %s\n", cmd.rstdin  ? cmd.rstdin  : "<none>" );
  printf("   stdout: %s\n", cmd.rstdout ? cmd.rstdout : "<none>" );
  printf("   bg    : %s\n", cmd.bakground ? "yes" : "no");
  PrintPgm(cmd.pgm);
}

void
PrintPgm (Pgm *p)
{
  if (p == NULL)
    return;
  else
  {
    char **pl = p->pgmlist;

    /* The list is in reversed order so print
     * it reversed so get right :-)
     */
    PrintPgm(p->next);
    printf("    [");
    while (*pl)
      printf("%s ", *pl++);
    printf("]\n");
  }
}

void
stripwhite (char *string)
{
  register int i = 0;
  while (whitespace( string[i] ))
    i++;
  if (i)
    strcpy (string, string + i);

  i = strlen( string ) - 1;
  while (i> 0 && whitespace (string[i]))
    i--;
  string [++i] = '\0';
}

