#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define SOCKET_PATH "/tmp/chat_socket3234"
#define BUFFER_SIZE 1024
int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len;
    // Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    // Remove old socket file if exists
    unlink(SOCKET_PATH);
    // Set server address
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen for connection
    if (listen(server_fd, 1) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on %s...\n", SOCKET_PATH);  
 client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client connected. Type 'exit' to stop.\n");
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE);
        // Check for exit condition from client
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client requested to exit. Shutting down...\n");
            break;
        }
        printf("Client: %s", buffer);
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        // Check for exit condition from server
        if (strncmp(buffer, "exit", 4) == 0) {
            write(client_fd, buffer, strlen(buffer));
            printf("Server shutting down...\n");
            break;
        }
        write(client_fd, buffer, strlen(buffer));
    }
    // Cleanup
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
