#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *client_socket) {
    int sock = *(int*)client_socket;
    free(client_socket);
    char buffer[BUFFER_SIZE];
    int read_size;

    // 클라이언트 요청을 읽음
    if ((read_size = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Received: %s\n", buffer);

        // 간단한 HTTP 응답
        char *http_response = 
"HTTP/1.1 200 OK\r\n"
									 "Server:Linux Web Server\r\n"
									 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
									 "<!DOCTYPE html>\r\n"
									 "<html><head><title> My Web Page </title>\r\n"
									 "<style>body {background-color: #FFFF00 }</style></head>\r\n"
									 "<body><center><h1>Hello world!!</h1><br>\r\n"
									 "<img src=\"/home/pi/Work1/Socket/learn06/game.jpg\"></center></body></html>\r\n";
        send(sock, http_response, strlen(http_response), 0);
    }

    close(sock);
    return NULL;
}

int main() {
    int server_fd, new_socket, *client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 연결 대기
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", PORT);

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

        // 새로운 스레드 생성
        if (pthread_create(&client_thread, NULL, handle_client, (void*)client_socket) < 0) {
            perror("could not create thread");
            close(new_socket);
            free(client_socket);
            continue;
        }

        // 스레드 분리
        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}
