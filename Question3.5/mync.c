#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

// Function to start a TCP server and return the new socket descriptor
int start_tcp_server(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    printf("11111111");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("socket created");
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("listening");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Started TCP server successfuly");
    return new_socket;
}

// Function to start a TCP client and return the socket descriptor
int start_tcp_client(const char *hostname, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct hostent *he;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    if ((he = gethostbyname(hostname)) == NULL) {
        perror("Invalid hostname");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    return sock;
}

// Function to relay data between input and output file descriptors
void relay_data(int input_fd, int output_fd) {
    char buffer[256];
    fd_set read_fds;
    int max_sd = input_fd > output_fd ? input_fd : output_fd;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(input_fd, &read_fds);
        FD_SET(output_fd, &read_fds);

        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select error");
            break;
        }

        // If data is available to read from input_fd
        if (FD_ISSET(input_fd, &read_fds)) {
            int bytes_read = read(input_fd, buffer, sizeof(buffer) - 1);  // reads terminal input
            if (bytes_read <= 0) {
                break;
            }
            buffer[bytes_read] = '\0';
            write(output_fd, buffer, bytes_read); // shows one terminal's input into the other one's output
        }

        // If data is available to read from output_fd
        if (FD_ISSET(output_fd, &read_fds)) {
            int bytes_read = read(output_fd, buffer, sizeof(buffer) - 1);  // reads terminal input
            if (bytes_read <= 0) {
                break;
            }
            buffer[bytes_read] = '\0';
            write(input_fd, buffer, bytes_read); // shows one terminal's input into the other one's output
        }
    }
}

int main(int argc, char *argv[]) {
    int fd = -1;
    int port;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <TCPS<PORT>|TCPC<IP>,<PORT>>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strncmp(argv[1], "TCPS", 4) == 0) {               // if TCPS....
        port = atoi(&argv[1][4]);                         // port = 4050
        printf("Starting TCP server on port %d\n", port);
        fd = start_tcp_server(port);                      // start server
        relay_data(fd, STDOUT_FILENO);
    } else if (strncmp(argv[1], "TCPC", 4) == 0) {
        port = atoi(&argv[1][4]);
        printf("Starting TCP client on local host on port %d\n", port);
        fd = start_tcp_client("127.0.0.1", port);
        relay_data(STDIN_FILENO, fd);
    } else {
        fprintf(stderr, "Invalid argument. Usage: %s <TCPS<PORT>|TCPC<IP>,<PORT>>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (fd != -1) {
        close(fd);
    }

    return 0;
}
