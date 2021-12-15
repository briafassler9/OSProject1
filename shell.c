#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/limits.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

// Author: Bria Fassler
// Worked with Nia Anderson and Darryle Mensah

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;
pid_t pid;
int *status;



int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    	
    while (true) {
      
        do{ 
            // Print the shell prompt.
            char cwd[PATH_MAX];
            if(getcwd(cwd, sizeof(cwd)) != NULL) {
              printf("%s> ", cwd);
            }
            fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			  
			
        // 1. Tokenize the command line input (split it on whitespace)
        int i = 0;
        arguments[i] = strtok(command_line, delimiters);
      
        while (arguments[i] != NULL){
          char *str = arguments[i];
          
          if (strchr(str, '$') != NULL){
            arguments[i] = getenv(str+1);
          }
          arguments[++i] = strtok(NULL, delimiters);
        }
      
        // 2. Implement Built-In Commands
        
        //cd: changes the current working directory
        if (strcmp(arguments[0], "cd") == 0){
          printf("%s \n", arguments[1]);
          chdir(arguments[1]);
        }
      
        //pwd: prints the current working directory
        else if (strcmp(arguments[0], "pwd") == 0){
          char cwd[PATH_MAX];
          if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s \n", cwd);
          }
        }
      
        //echo: prints a message and the values of environment variables
        else if (strcmp(arguments[0], "echo") == 0){
            int j = 1;
            while (arguments[j] != NULL) {
              printf("%s", arguments[j]);
              j++;
            }
          printf("\n");
          }
      
        //exit: terminates the shell
        else if (strcmp(arguments[0], "exit") == 0){
          return 0;
        }
      
        //env: prints the current values of the environment variables
        else if (strcmp(arguments[0], "env") == 0){
          int k = 1;
          char *e = *environ;
          for (; e; k++){
            printf("%s\n", e);
            e = *(environ + k);
          }
        }
      
        //setenv: sets an environment variable
        else if (strcmp(arguments[0], "setenv") == 0){
          setenv(arguments[1], arguments[2], 1);
        }
    
        // 3. Create a child process which will execute the command line input
        else {
          pid = fork();
          if (pid < 0) {
            printf("error\n");
            exit(1);
          }
          
          else if (pid == 0){
            char *args[] = {arguments[0], arguments[1], arguments[2], NULL};
            execvp(args[0], args);
            exit(0);
            
            if(strcmp(arguments[1], "&") == 0){
              char *args[] = {arguments[0], NULL};
              execvp(args[0], args);
            }
          }
        }
      
  
        // 4. The parent process should wait for the child to complete unless its a background process
        pid = wait(status);
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}