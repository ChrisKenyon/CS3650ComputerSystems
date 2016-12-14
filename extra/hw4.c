/* See Chapter 5 of Advanced UNIX Programming:  http://www.basepath.com/aup/
 *   for further related examples of systems programming.  (That home page
 *   has pointers to download this chapter free.
 *
 * Copyright (c) Gene Cooperman, 2006; May be freely copied as long as this
 *   copyright notice remains.  There is no warranty.
 */

/* To know which "includes" to ask for, do 'man' on each system call used.
 * For example, "man fork" (or "man 2 fork" or man -s 2 fork") requires:
 *   <sys/types.h> and <unistd.h>
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAXLINE 200  /* This is how we declare constants in C */
#define MAXARGS 20
#define PIPE_READ 0
#define PIPE_WRITE 1

/* In C, "static" means not visible outside of file.  This is different
 * from the usage of "static" in Java.
 * Note that end_ptr is an output parameter.
 */
static char * getword(char * begin, char **end_ptr) {
    char * end = begin;

    while ( *begin == ' ' )
        begin++;  /* Get rid of leading spaces. */
    end = begin;
    while ( *end != '\0' && *end != '\n' && *end != ' ' )
        end++;  /* Keep going. */
    if ( end == begin )
        return NULL;  /* if no more words, return NULL */
    *end = '\0';  /* else put string terminator at end of this word. */
    *end_ptr = end;
    if (begin[0] == '$') { /* if this is a variable to be expanded */
        begin = getenv(begin+1); /* begin+1, to skip past '$' */
        if (begin == NULL) {
            perror("getenv");
            begin = "UNDEFINED";
        }
    }
    return begin; /* This word is now a null-terminated string.  return it. */
}

/* In C, "int" is used instead of "bool", and "0" means false, any
 * non-zero number (traditionally "1") means true.
 */
/* argc is _count_ of args (*argcp == argc); argv is array of arg _values_*/
static void getargs(char cmd[], int *argcp, char *argv[], char** file_input, char** file_output, int *pipe, int *bg)
{
    char *cmdp = cmd;
    char *end;
    int i = 0;

    /* fgets creates null-terminated string. stdin is pre-defined C constant
     *   for standard intput.  feof(stdin) tests for file:end-of-file.
     */
    if (fgets(cmd, MAXLINE, stdin) == NULL && feof(stdin)) {
        printf("Couldn't read from standard input. End of file? Exiting ...\n");
        exit(1);  /* any non-zero value for exit means failure. */
    }
    while ( (cmdp = getword(cmdp, &end)) != NULL && *cmdp != '#') { /* end is output param */

        if (*cmdp == '|'){
            *pipe = 1;
        }
        else if (*cmdp == '<'){
            cmdp = end + 1;
            *file_input = getword(cmdp,&end);
        }
        else if (*cmdp == '>'){
            cmdp = end + 1;
            *file_output = getword(cmdp,&end);
        }
        else if (*cmdp == '&'){
            *bg = 1;
            break;
        }
        else{
            /* getword converts word into null-terminated string */
            argv[i++] = cmdp;
        }
        /* "end" brings us only to the '\0' at end of string */
        cmdp = end + 1;
    }
    argv[i] = NULL; /* Create additional null word at end for safety. */
    *argcp = i;
}

/* Much of this function was taken from pipe-example.c */
static void execute(int argc, char *argv[], char* file_input, char* file_output, int isPipe)
{
    int fd;
    int pfd[2];
    pid_t child1, child2; /* child process ID */
    if (isPipe){
        if (-1 == pipe(pfd)) {
          perror("pipe");
        }
    }
    child1 = fork();
    if (child1 > 0 && isPipe) /* in parent, need to fork for pipe */
        child2 = fork();
    if (child1 == -1) { /* in parent (returned error) */
        perror("fork"); /* perror => print error string of last system call */
        printf("  (failed to execute command)\n");
    }
    if (child1 == 0) { /* child:  in child, childpid was set to 0 */
        if (strlen(file_output)>0){
            close(STDOUT_FILENO);
            /* This causes any output to stdout goes to output_file: see 'man open' */
            fd = open(file_output, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1) perror("open for writing");
        }
        else if (strlen(file_input)>0){
            close(STDIN_FILENO);
            /* This causes any input from stdin to comre from input_file: see 'man open' */
            fd = open(file_input, O_RDONLY);  /* Now any read from stdin reads from input_file. */
            if (fd == -1) perror("open for reading");
        }
        else if (isPipe) {
            /* close */
            if (-1 == close(STDOUT_FILENO)) {
              perror("close");
            }
            fd = dup(pfd[PIPE_WRITE]); /* set up empty STDOUT to be pfd[1] */
            if (-1 == fd) {
                perror("dup");
            }
            if (fd != STDOUT_FILENO) {
                fprintf(stderr, "Pipe output not at STDOUT.\n");
            }

            close(pfd[PIPE_READ]); /* never used by child1 */
            close(pfd[PIPE_WRITE]); /* not needed any more */

            argv[1] = NULL;
        }

        printf("%s",argv[0]);
        if (-1 == execvp(argv[0], argv)) {
            perror("execvp");
            printf("  (couldn't find command)\n");
        }
        /* NOT REACHED unless error occurred */
        exit(1);
    } else if (child2 == 0 && isPipe) {
        /* close */
        if (-1 == close(STDIN_FILENO)) {
            perror("close");
        }

        /* set up empty STDIN to be pfd[0] */
        fd = dup(pfd[PIPE_READ]);
        if (-1 == fd) {
            perror("dup");
        }

        if (fd != STDIN_FILENO) {
            fprintf(stderr, "Pipe input not at STDIN.\n");
        }

        close(pfd[PIPE_READ]); /* not needed any more */
        close(pfd[PIPE_WRITE]); /* never used by child2 */

        argv[0] = argv[1];
        argv[1] = NULL;
        if (-1 == execvp(argv[0], argv)) {
            perror("execvp");
            printf("  (couldn't find command)\n");
        }
    } else { /* parent:  in parent, childpid was set to pid of child process */
        int status;
        if (-1 == waitpid(child1, &status, 0)) {
            perror("waitpid");
        }

        if (isPipe){
            close(pfd[PIPE_READ]);
            close(pfd[PIPE_WRITE]);


            /* Optionally, check return status.  This is what main() returns. */
            if (WIFEXITED(status) == 0) {
                printf("child1 returned w/ error code %d\n", WEXITSTATUS(status));
            }

            if (-1 == waitpid(child2, &status, 0)) {
                perror("waitpid");
            }

            /* Optionally, check return status.  This is what main() returns. */
            if (WIFEXITED(status) == 0) {
                printf("child2 returned w/ error code %d\n", WEXITSTATUS(status));
            }
        }
    }
    return;
}

void interrupt_handler(int signum)
{
    // Don't let interrupt exit this program
    printf("Signal caught: %i. If trying to kill, please type 'exit'\n",signum);
}

int main(int argc, char *argv[])
{
    signal(SIGINT,interrupt_handler);

    if (argc > 1)
        freopen(argv[1], "r", stdin);

    // init execution args and flags
    char cmd[MAXLINE];
    char *childargv[MAXARGS];
    int childargc;
    char *file_input = "";
    char *file_output = "";
    int pipe = 0;
    int bg = 0;
    pid_t bgPid;

    while (1) {
        printf("%% "); /* printf uses %d, %s, %x, etc.  See 'man 3 printf' */
        fflush(stdout); /* flush from output buffer to terminal itself */
        getargs(cmd, &childargc, childargv, &file_input, &file_output, &pipe, &bg); /* childargc and childargv are
                output args; on input they have garbage, but getargs sets them. */
            /* Check first for built-in commands. */
        if ( childargc > 0 && strcmp(childargv[0], "exit") == 0 )
            exit(0);
        else if ( childargc > 0 && strcmp(childargv[0], "logout") == 0 )
            exit(0);
        else {
            // if we should be running in the background, then fork here before executing
            if (bg){
                bgPid = fork();
                if (bgPid == 0){
                    // and only execute on the child
                    execute(childargc, childargv, file_input, file_output, pipe);
                    break;
                }
            }
            // otherwise just execute it
            else execute(childargc, childargv, file_input, file_output, pipe);
            // reset the flags and file names
            file_output = file_input = "";
            pipe = 0;
            bg = 0;
        }
    }
    /* NOT REACHED */
}
