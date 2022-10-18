#include "socket.h"

static void element_destroy (void* item)
{
    if(item != NULL)
    {
        free(item);
        item = NULL;
    }
}

static void load_frome_file(char* server_ip, int* server_port)
{
    FILE* file;
    file = fopen("../resources/server_ip_and_port.txt", "r");
    if(fopen == NULL)
    {
        strcpy(server_ip, "0.0.0.0");
        *server_port = 5555;
        perror("fopen fail");
    }

    fscanf(file ,"%s%d", server_ip, server_port);
    fclose(file);
}

static void set_sin(struct sockaddr_in* sin, Socket* socket)
{
	load_frome_file(socket->m_server_ip, &socket->m_server_port);

	memset (sin, 0, sizeof(*sin));	
	(*sin).sin_family = AF_INET;
	(*sin).sin_addr.s_addr =  inet_addr(socket->m_server_ip);
	(*sin).sin_port = htons(socket->m_server_port);
}

static int open_listen_socket(int* listen_socket, fd_set* set)
{
	*listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(*listen_socket < 0)
    {
		perror("open_listen_socket fail");
        return FAIL;
    }

	FD_ZERO(set);
	FD_SET(*listen_socket, set);
    return SUCCESS;
}

static int set_listen_socket(int* listen_socket)
{
	if(listen(*listen_socket, LISTEN_QUEUE_SIZE) < 0)
    {
		perror("set_listen_socket fail");
        return FAIL;
    }

    return SUCCESS;
}

static int set_setsockopt(int* listen_socket)
{
    int optval = 1;		
	if(setsockopt(*listen_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
		perror("set_setsockopt fail");
        return FAIL;
    }

    return SUCCESS;
}

static int set_bind(int* listen_socket, struct sockaddr_in* sin)
{
	if(bind(*listen_socket, (struct sockaddr*)sin, sizeof(*sin)) < 0)
    {
		perror("set_bind fail");
        return FAIL;
    }

    return SUCCESS;
}

Socket* socket_create()
{
    Socket* socket;
    if((socket = (Socket*)malloc(sizeof(Socket))) == NULL)
    {
        perror("malloc socket_create fail");
        return NULL;
    }

    socket->m_active_fd = (fd_set*)malloc(sizeof(fd_set));
    if(socket->m_active_fd == NULL)
    {
        free(socket);
        return NULL;
    }

    socket->m_client_sin = (struct sockaddr*)malloc(sizeof(struct sockaddr));
    if(socket->m_client_sin == NULL)
    {
        free(socket->m_active_fd);
        free(socket);
        return NULL;
    }

    set_sin(&socket->m_server_sin, socket);

	if(!open_listen_socket(&socket->m_listen_socket, &socket->m_source_fd))
    {
        free(socket->m_active_fd);
        free(socket);
        return NULL;
    }
      
	if(!set_setsockopt(&socket->m_listen_socket))
    {
        free(socket->m_active_fd);
        free(socket);
        return NULL;
    }

	if(!set_bind(&socket->m_listen_socket, &socket->m_server_sin))
    {
        free(socket->m_active_fd);
        free(socket);
        return NULL;
    }

	if(!set_listen_socket(&socket->m_listen_socket))
    {
        free(socket->m_active_fd);
        free(socket);
        return NULL;
    }

    socket->m_connected_sockets = list_create();
    if(socket->m_connected_sockets == NULL)
    {
        perror("ListCreate fail: \n");
        free(socket->m_active_fd);
        free(socket);
        return NULL;
    }

    socket->m_deleted_sockets = list_create();
    if(socket->m_deleted_sockets == NULL)
    {
        perror("ListCreate fail");
        list_destroy(&socket->m_connected_sockets, element_destroy);
        free(socket->m_active_fd);
        free(socket);
        return NULL;
    }

    socket->m_num_of_clients = 0;
    return socket;
}

void socket_destroy(Socket* socket)
{
    if(socket != NULL)
    {
        list_destroy(&socket->m_deleted_sockets, element_destroy);
        list_destroy(&socket->m_connected_sockets, element_destroy);
        free(socket->m_active_fd);
        free(socket->m_client_sin);
        close_all_clients_sockets(socket);
        close(socket->m_listen_socket);
        free(socket);
        socket = NULL;
    }
}

int num_of_clients(Socket* socket)
{
    if(socket == NULL)
        return SOCKET_NOT_INITIALIZE;

    return socket->m_num_of_clients;
}

int listen_socket(Socket* socket)
{
    if(socket == NULL)
        return SOCKET_NOT_INITIALIZE;

    return socket->m_listen_socket;
}

struct sockaddr* client_sin(Socket* socket)
{
    if(socket == NULL)
        return NULL;

    return socket->m_client_sin;
}

int is_in_active_fd(Socket* socket, int socket_num)
{
    if(socket == NULL)
        return FAIL;

    return FD_ISSET(socket_num, socket->m_active_fd);
}

fd_set* copy_of_source_fd(Socket* socket)
{
    if(socket == NULL)
        return NULL;

    FD_ZERO(socket->m_active_fd);
    *socket->m_active_fd = socket->m_source_fd;
    return socket->m_active_fd;
}

List* deleted_sockets(Socket* socket)
{
    if(socket == NULL)
        return NULL;

    return socket->m_deleted_sockets;
}

List* connected_sockets(Socket* socket)
{
    if(socket == NULL)
        return NULL;

    return socket->m_connected_sockets;
}

void delete_less_active_client(Socket* socket)
{
    if(socket == NULL)
        return;

    ListItr it = end(socket->m_connected_sockets);
    it = prev(it);
    delete_client(socket, it);
}

static void push_to_list(List* list, int client_socket)
{
    int* client_socket_ptr = (int*)malloc(sizeof(int));
    *client_socket_ptr = client_socket;
    list_push_head(list, (void*)(client_socket_ptr));
}

void insert_client(Socket* socket, int client_socket)
{
    if(socket == NULL)
        return;

    FD_SET(client_socket, &socket->m_source_fd);
    push_to_list(socket->m_connected_sockets, client_socket);
    socket->m_num_of_clients++;
}

void delete_client(Socket* socket, ListItr it)
{
    if(socket == NULL || it == NULL)
        return;
    
    printf("+ delete client: %d\n", *(int*)get_data(it));

    int client_socket = *(int*)get_data(it);
	close(client_socket); 
    FD_CLR(client_socket, &socket->m_source_fd);
    free(remove_it(socket->m_connected_sockets, it));
    push_to_list(socket->m_deleted_sockets, client_socket);
    socket->m_num_of_clients--;
}

void move_client_to_front(Socket* socket, ListItr it)
{
    if(socket == NULL || it == NULL)
        return;

    int client_socket = *(int*)get_data(it);
    free(remove_it(socket->m_connected_sockets, it));
    push_to_list(socket->m_connected_sockets, client_socket);
}

void delete_from_deleted_sockets(Socket* socket, ListItr it)
{
    if(socket == NULL || it == NULL)
        return;

    free(remove_it(socket->m_deleted_sockets, it));
}

void close_all_clients_sockets(Socket* socket)
{
    if(socket == NULL)
        return;

	ListItr it = begin(socket->m_connected_sockets);
	ListItr end_it = end(socket->m_connected_sockets);
    while(it != end_it)
    {
        close(*(int*)get_data(it));
        it  = next(it);
    }
}

char* server_ip(Socket* socket)
{
    return socket->m_server_ip;
}

int server_port(Socket* socket)
{
    return socket->m_server_port;
}
