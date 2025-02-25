#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define SOCKET_PATH "/tmp/chat_socket3234"
#define BUFFER_SIZE 1024
int main() {
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    // Create socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    // Set server address
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server. Type 'exit' to stop.\n");
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        // Check if client wants to exit
        if (strncmp(buffer, "exit", 4) == 0) {
            write(client_fd, buffer, strlen(buffer)); // Inform server
            printf("Client exiting...\n");
            break;
}
        write(client_fd, buffer, strlen(buffer));
        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE);
        // Check if server sent exit message
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server requested to exit. Closing connection...\n");
            break;
        }
        printf("Server: %s", buffer);
    }
    close(client_fd);
    return 0;
}
