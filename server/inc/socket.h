#ifndef SOCKET_H
#define SOCKET_H

#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "list_iterator.h"

#define SOCKET_NOT_INITIALIZE -1
#define LISTEN_QUEUE_SIZE 1000
#define SUCCESS 1
#define FAIL 0
#define TRUE 1
#define FALSE 0

typedef struct Socket
{
    int m_listen_socket;
    int m_num_of_clients;

    fd_set m_source_fd;
    fd_set* m_active_fd;

    struct sockaddr_in m_server_sin;
    struct sockaddr* m_client_sin;
    
    List* m_connected_sockets;
    List* m_deleted_sockets;

    char m_server_ip[20];
    int m_server_port;

} Socket;

Socket* socket_create();

void socket_destroy(Socket* socket);

int num_of_clients(Socket* socket);

int listen_socket(Socket* socket);

struct sockaddr* client_sin(Socket* socket);

int is_in_active_fd(Socket* socket, int socket_num);

fd_set* copy_of_source_fd(Socket* socket);

List* deleted_sockets(Socket* socket);

List* connected_sockets(Socket* socket);

void delete_less_active_client(Socket* socket);

void insert_client(Socket* socket, int client_socket);

void delete_client(Socket* socket, ListItr it);

void move_client_to_front(Socket* socket, ListItr it);

void delete_from_deleted_sockets(Socket* socket, ListItr it);

void close_all_clients_sockets(Socket* socket);

char* server_ip(Socket* socket);

int server_port(Socket* socket);


#endif //SOCKET_H
