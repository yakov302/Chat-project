#include "socket.h"

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

static int set_socket(Socket* socket_struct)
{
	socket_struct->m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_struct->m_socket < 0)
    {
		perror("Set socket fail");
        return FAIL;
    }

    FD_ZERO(socket_struct->m_fd);
    FD_SET(socket_struct->m_socket, socket_struct->m_fd);
    return SUCCESS;
}

static int set_setsockopt(int* socket)
{
    int optval = 1;		
	if(setsockopt(*socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
		perror("set_setsockopt fail");
        return FAIL;
    }

    return SUCCESS;
}

static int connect_to_server(struct sockaddr_in* a_sin, int a_socket)
{
    if(connect(a_socket, (struct sockaddr*)a_sin, sizeof(*a_sin)) < 0)
    {
		perror("Connect to server fail!\n");
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

    socket->m_fd = (fd_set*)malloc(sizeof(fd_set));
    if(socket->m_fd == NULL)
    {
        free(socket);
        return NULL;
    }

    socket->m_sin = (struct sockaddr*)malloc(sizeof(struct sockaddr));
    if(socket->m_sin == NULL)
    {
        free(socket->m_fd);
        free(socket);
        return NULL;
    }

    set_sin((struct sockaddr_in*)socket->m_sin, socket);
      
    if(!set_socket(socket))
    {
        free(socket->m_sin);
        free(socket->m_fd);
        free(socket);
        return NULL;
    }

	if(!set_setsockopt(&socket->m_socket))
    {
        free(socket->m_sin);
        free(socket->m_fd);
        free(socket);
        return NULL;
    }

    if(!connect_to_server((struct sockaddr_in*)socket->m_sin, socket->m_socket))
    {
        free(socket->m_sin);
        free(socket->m_fd);
        free(socket);
        return NULL;
    }
    
    return socket;
}

void socket_destroy(Socket* socket)
{
    if(socket != NULL)
    {
        close(socket->m_socket);
        free(socket->m_sin);
        free(socket->m_fd);
        free(socket);
        socket = NULL;
    }
}

int my_socket(Socket* socket)
{
    if(socket == NULL)
        return SOCKET_NOT_INITIALIZE;

    return socket->m_socket;
}

fd_set* fd(Socket* socket)
{
    if(socket == NULL)
        return NULL;
    return socket->m_fd;
}

