#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

//  mync -e "ttt 123456789"
// This main should work only on the games from Q1.
int main(int argc, char* argv[]){
    if (argc != 3){
        printf("usgae : 'mync -e '123456789'\n");
        exit(1);
    }
    
    pid_t pid = fork();

    if (pid < 0){
        perror("fork");
        return 1;
    }

    char* path = "../Question1/ttt";
    char* exec_args[] = {path, argv[2], NULL};

    if (pid == 0){   // child process
        execvp(path, exec_args);
        perror("execvp");
        exit(1);
    }

    if (pid > 0){   // parent process
        waitpid(pid, NULL, 0);   // wait for the child process to finish
    }
    
    return 0;
}

