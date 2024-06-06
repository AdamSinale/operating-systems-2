#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h> // Include the netdb.h header file
#include <signal.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>

int start_tcp_server(int port, char mode, char args[9]){
    printf("server received mode %c\n", mode);
    printf("bot will play %s\n", args);

    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("accepted with mode %c\n", mode);
    sleep(1);
    write(new_socket, &mode, 1);
    sleep(1);

    pid_t pid = fork();
    if (pid < 0) {
        printf("fork failed");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        switch (mode) {
            case 'i':
                dup2(new_socket, STDIN_FILENO);
                break;
            case 'o':
                dup2(new_socket, STDOUT_FILENO);
                break;
            case 'b':
                dup2(new_socket, STDIN_FILENO);
                dup2(new_socket, STDOUT_FILENO);
                break;
        }
        close(server_fd);
        
        char *args1[] = {"ttt", args, NULL};
        char *path = "../Question1/ttt";
        execvp(path, args1);
        printf("child came back\n");

        // If execvp fails
        perror("execvp failed"); exit(EXIT_FAILURE);
    } else {
        printf("parent process\n");
        waitpid(pid, NULL, 0);
        printf("game finished\n");
        close(new_socket);
    }

    close(server_fd);
    return 0;    
}

int start_tcp_client(const char *hostname, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct hostent *he;
    char buffer[255];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    if ((he = gethostbyname(hostname)) == NULL) {
        printf("\nInvalid hostname\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }
    printf("connection made, put input:\n");
    // Make the socket non-blocking
    memset(buffer, 0, sizeof(buffer));
    size_t bytesRead = read(sock, buffer, sizeof(buffer)-1);
    if(bytesRead < 1){
        printf("problem receiving mode");
        return -1;
    }
    char mode = buffer[0];
    printf("servers mode is %c\n", mode);
    while (1) {
        if(mode == 'b'){
            printf("\n");
            memset(buffer, 0, sizeof(buffer));
            size_t bytesRead = read(sock, buffer, sizeof(buffer)-1);
            buffer[bytesRead] = '\0';
            if(bytesRead <= 0){
                printf("problem receiving");
                return -1;
            }
            if(bytesRead == 1){ break; }
            printf("%s\n",buffer);
            if(buffer[bytesRead-2] == 'n' || buffer[bytesRead-2] == 'W' || buffer[bytesRead-2] == 't'){ break; }
        }
        printf("Enter position to play (single character). (0 for exit)\n");
        char input = getchar();
        getchar(); // Consume the newline character
        write(sock, &input, 1); // Send a single character
        if (input == '0') { // Check for exit condition
            break;
        }
        printf("Sent: %c\n", input);
        fflush(stdout);
    }
    close(sock);
    return 0;
}

int main(int argc, char *argv[]){
    if (argc < 3){
        fprintf(stderr, "Server Usage1: %s -e \"ttt 123456789\" -i TCPS4050\n", argv[0]);
        fprintf(stderr, "Server Usage2: %s -e \"ttt 123456789\" -b TCPS4050\n", argv[0]);
        fprintf(stderr, "Client Usage: %s -e \"ttt 123456789\" -o TCPClocalhost,4050\n", argv[0]);
        return 1;
    }

    int both_fd = -1;
    char mode = *(argv[3] + 1);
    printf("mode: %c\n", mode);
    fflush(stdout);
    char command[9];

    for (int i = 1; i < argc; ++i){
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc){
            strncpy(command, argv[2] + 4, 10);
            command[9] = '\0';
        }
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc){
            printf("received i\n");
            if (strncmp(argv[i + 1], "TCPS", 4) == 0) {
                int port = atoi(argv[++i] + 4);
                start_tcp_server(port, mode, command);
                printf("starting server with port: %d", port);
            }
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc){
            if (strncmp(argv[i + 1], "TCPC", 4) == 0){
                char *host_port = argv[++i] + 4;
                char *host = strtok(host_port, ",");
                if (host != NULL){
                    int port = atoi(strtok(NULL, ","));
                    start_tcp_client(host, port);
                }
            }
        }
        else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc){
            if (strncmp(argv[i + 1], "TCPS", 4) == 0){
                int port = atoi(argv[++i] + 4);
                both_fd = start_tcp_server(port, mode, command);
            }
        }
    }
    dup2(both_fd, STDIN_FILENO);
    dup2(both_fd, STDOUT_FILENO);
}
