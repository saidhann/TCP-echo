#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define PORT 8080
#define INITIAL_BUFFER_SIZE 1024

int server_sock;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Signal handler to close the sock before closing process
void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    close(server_sock);
    exit(0);
}

// Thread-safe logging handler
void safe_log(const char *message) {
    pthread_mutex_lock(&log_mutex);
    printf("%s\n", message);
    pthread_mutex_unlock(&log_mutex);
}

// Client handler
void *handle_client(void *client_socket) {
    int sock = *(int*)client_socket;
    free(client_socket);
    int buffer_size = INITIAL_BUFFER_SIZE;
    char *buffer = malloc(buffer_size);

    if (buffer == NULL) {
        perror("Failed to allocate buffer");
        close(sock);
        return NULL;
    }

    int read_size;
    while ((read_size = recv(sock, buffer, buffer_size - 1, 0)) > 0) {
        buffer[read_size] = '\0';

        if (strcmp(buffer, "disconnect") == 0) {
            safe_log("Client disconnected.");
            break;
        }

        if (strcmp(buffer, "hello") == 0) {
            send(sock, "world\n", strlen("world\n"), 0);
        } else {
            send(sock, buffer, read_size, 0);
        }

        if (read_size == buffer_size - 1) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (new_buffer == NULL) {
                perror("Failed to resize buffer");
                break;
            }
            buffer = new_buffer;
        }
    }

    if (read_size == 0) {
        safe_log("Client disconnected.");
    } else if (read_size == -1) {
        perror("recv failed");
    }

    free(buffer);
    close(sock);
    return NULL;
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    //Important for clean up
    signal(SIGINT, handle_sigint);  // Handle shutdown to close the sock before closing process

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    safe_log("Server listening on port 8080...");

    while (1) {
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        safe_log("Connection accepted");

        pthread_t client_thread;
        int *new_sock = malloc(sizeof(int));
        if (new_sock == NULL) {
            perror("Failed to allocate memory for client socket");
            close(client_sock);
            continue;
        }
        *new_sock = client_sock;

        if (pthread_create(&client_thread, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
            close(client_sock);
            continue;
        }

        pthread_detach(client_thread);
    }

    close(server_sock);

    //Important for clean up
    pthread_mutex_destroy(&log_mutex); //Important Im learing from past mistakes
    return 0;
}