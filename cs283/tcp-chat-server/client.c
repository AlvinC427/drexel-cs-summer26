#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>

#define SERVER_HOST "tux5"
#define SERVER_PORT 2026
#define BUFSIZE 1024

void *receive(void *);

int
main(int argc, char *argv[]) {
    char *name;
    int sock;
    struct sockaddr_in serverAddr;
    struct hostent *server;
    pthread_t tid;
    char buf[BUFSIZE];

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        exit(1);
    }
    name = argv[1];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    server = gethostbyname(SERVER_HOST);
    if(server == NULL) {
        fprintf(stderr, "No such host '%s'\n", SERVER_HOST);
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);
    if(connect(sock, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in)) < 0) {
        perror("Connection to server failed");
        exit(1);
    }
    printf("Successfully connected to %s on port %d as %s!\n", SERVER_HOST, SERVER_PORT, name);

    send(sock, name, strlen(name), 0);
    if(pthread_create(&tid, NULL, receive, &sock) != 0) {
        perror("pthread_create");
        exit(1);
    }

    while(fgets(buf, BUFSIZE, stdin) != NULL) {
        send(sock, buf, strlen(buf), 0);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);
    return 0;
}

void *receive(void *arg) {
    char buf[BUFSIZE];
    int s;
    int n;

    s = *(int *)arg;
    while((n = recv(s, buf, BUFSIZE - 1, 0)) > 0) {
        buf[n] = '\0';
        fputs(buf, stdout);
    }

    // if recv() returns 0 or less
    printf("\nServer disconnected.\n");
    exit(0);
    return NULL;
}
