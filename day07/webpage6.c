#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

const char webpage[] = "HTTP/1.1 200 OK\r\n"
                       "Server: Linux Web Server\r\n"
                       "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                       "<!DOCTYPE html>\r\n"
                       "<html><head><title> My Web Page </title>\r\n"
                       "<style>body {background-color: #F4A460 }</style></head>\r\n"
                       "<body><center><h1>Hello cat!!</h1><br>\r\n"
                       "<img src=\"game.jpg\"></center></body></html>\r\n";


void *handle_client(void *client_socket) {
    int sock = *(int*)client_socket;
    free(client_socket);
    char buffer[BUFFER_SIZE];
    int read_size;

    
    if ((read_size = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Received: %s\n", buffer);

        if (strstr(buffer, "GET /game.jpg") != NULL) {
    
            FILE *image_file = fopen("game.jpg", "rb");
            if (image_file == NULL) {
                perror("Failed to open image file");
                close(sock);
                return NULL;
            }

    
            struct stat st;
            stat("game.jpg", &st);
            int file_size = st.st_size;

    
            char header[BUFFER_SIZE];
            snprintf(header, sizeof(header),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: image/jpeg\r\n"
                     "Content-Length: %d\r\n"
                     "\r\n", file_size);

    
            send(sock, header, strlen(header), 0);

    
            char file_buffer[BUFFER_SIZE];
            size_t bytes_read;
            while ((bytes_read = fread(file_buffer, 1, BUFFER_SIZE, image_file)) > 0) {
                send(sock, file_buffer, bytes_read, 0);
            }

            fclose(image_file);
        } else {
    
            send(sock, webpage, strlen(webpage), 0);
        }
    }

    close(sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number: %d\n", port);
        exit(EXIT_FAILURE);
    }

    int server_fd, new_socket, *client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", port);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        pthread_t client_thread;
        client_socket = malloc(sizeof(int));
        *client_socket = new_socket;


        if (pthread_create(&client_thread, NULL, handle_client, (void*)client_socket) < 0) {
            perror("could not create thread");
            close(new_socket);
            free(client_socket);
            continue;
        }


        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}
