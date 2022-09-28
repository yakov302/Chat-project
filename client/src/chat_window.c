#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256
#define IP_SIZE 20
#define TRUE 1
#define FALSE 0

static void save_process_id_to_file()
{	    	
	FILE* file = fopen("chat_window_process_id.txt", "w");
	if(file == NULL)
	{
		printf("fopen fail!\n");
		return;
	}

    pid_t process_id = getpid();
	fprintf(file,"%d\n", process_id);	
	fclose(file);
}

static void set_sin(struct sockaddr_in* sin, char* ip, int port)
{
    memset(sin, 0, sizeof(*sin));
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(ip);
    sin->sin_port = htons(port);
}

static int udp_server_init(int* socket_number, struct sockaddr_in* sin, char* ip, int port)
{
    *socket_number = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_number < 0) 
    {
        perror("open_socket failed!");
        return FALSE;
    }

    int optval = 1;
    if (setsockopt(*socket_number, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
       perror("setsockopt fail!");
       return FALSE;
    }

    set_sin(sin, ip, port);
    if(bind(*socket_number, (struct sockaddr*)sin, sizeof(*sin)) < 0) 
    {
        perror("bind fail!");
        return FALSE;
    }

    return TRUE;
}

int main(int argc, char* argv[])
{  
    if(argc < 3) {printf("IP and PORT required\n"); return FALSE;}

    int socket_number;
    struct sockaddr_in sin;
    if(!udp_server_init(&socket_number, &sin, argv[1], atoi(argv[2])))
        return FALSE;

    save_process_id_to_file();

    char buffer[BUFFER_SIZE];
    socklen_t sin_len = sizeof(sin);

    while (TRUE) 
    {
        int receive_bytes  = recvfrom(socket_number, buffer, 5, 0, (struct sockaddr*)&sin, &sin_len);
        if (receive_bytes < 0) 
        {
            perror("recvfrom fail");
            return FALSE;
        }
        printf("resiv\n");
        buffer[receive_bytes] = '\0';
        puts(buffer);
    }

    return TRUE;
}
