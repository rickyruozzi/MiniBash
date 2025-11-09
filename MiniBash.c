#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void parse_input(char* input, char **args){
    int i = 0; //iterator
    args[i] = strtok(input, " \n"); //tokenizing the string received as input
    //args[0] will contain the command
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        i++; 
        args[i] = strtok(NULL, " \n"); //every other args will contain the arguments
    }
    //now we have the command and its argument in the args array
}

int main(){
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    while(1){
        printf("\033[31mMinBash>\033[0m "); //display in red
        fflush(stdout); //flushing the buffer
        if(!fgets(input,sizeof(input), stdin)){
            break; //exit when no string is left
        }
        parse_input(input,args);
        if(args[0]==NULL){
            continue; //skip the while loop if the command is not given
        }
        pid_t pid=fork(); //we are creating a child process of the bash
        if(pid<0){
            perror("fork failed");
            continue; //if the fork failed skip the iteration
        }
        if(pid==0){
            //we have obtained the child process
            if(execvp(args[0],args)<0){
                perror("exec failed"); //we use execvp to execute the command with its arguments
                /* we need to pass the command as the first parameter and then the array of arguments
                we don't have to remove the command name because the system call is expecting to find it
                in the first position, so it will skip it automatically.*/
                exit(EXIT_FAILURE); //exit the child process if exec fails
            }
        }
        else if(pid>0){
            waitpid(pid,NULL,0); //after the fork the parent process will wait here for the child to finish
        }
    }
    return 0;
}
