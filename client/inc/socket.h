#ifndef SOCKET_H
#define SOCKET_H

#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define SOCKET_NOT_INITIALIZE -1
#define SUCCESS 1
#define FAIL 0
#define TRUE 1
#define FALSE 0

typedef struct Socket
{
    int m_socket;
    fd_set* m_fd;
    struct sockaddr* m_sin;

    char m_server_ip[20];
    int m_server_port;

} Socket;

Socket* socket_create();

void socket_destroy(Socket* socket);

int my_socket(Socket* socket);

fd_set* fd(Socket* socket);


#endif //SOCKET_H
