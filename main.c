#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>

char *backgrnd;
int background; background=1;

void LogHandler(int sig)
{   pid_t  pid;
    int status;
    pid = wait(&status);

    FILE *fp;
    fp=fopen("test.txt","a");
   fprintf(fp, "Child process was terminated.\n");

   fclose(fp);

}

void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */
          while (*line == ' ' || *line == '\t' || *line == '\n')
                *line++ = '\0';     /* replace white spaces with 0    */
                *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' &&
                 *line != '\t' && *line != '\n')
                line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv)
{

     pid_t  pid;
     int status;
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {
          if(background){                 /* for the parent:      */
          waitpid(pid, &status, 0);
                        }                /* wait for completion  */

     }
}

void  main(void)
{    char  line[1024];             // Memory allocation for input
     char  *argv[64];

     FILE *fp;                     // Clearing log file
     fp = fopen("test.txt", "w");
     fclose(fp);

     signal(SIGCHLD,LogHandler);   // Child termination signal handler

     while (1) {


          printf("SimpleShell -> ");
          gets(line);
          printf("\n");

          backgrnd = strchr(line, '&');                // Detect '&' symbol to enable background process
          if (backgrnd == NULL){background = 1;}
          else {background = 0;*backgrnd=NULL;backgrnd--;*backgrnd=NULL;}

          parse(line, argv);       // Calls parse function to prepare arguments
          if (strcmp(argv[0], "exit") == 0)  // Checks for exit command
               exit(0);
          execute(argv); // Executes arguments

     }
}
