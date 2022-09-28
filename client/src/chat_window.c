#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
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

static void set_multicast( struct ip_mreq* mreq, char* ip)
{
    memset(mreq, 0, sizeof(*mreq));
    mreq->imr_multiaddr.s_addr = inet_addr(ip);
    mreq->imr_interface.s_addr = htonl(INADDR_ANY);
}

static int udp_server_init(int* socket_number, struct sockaddr_in* sin, char* ip, int port)
{
    *socket_number = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_number < 0) 
    {
        perror("open_socket failed!");
        return FALSE;
    }

    struct ip_mreq mreq;
    set_multicast(&mreq, ip);
    if (setsockopt(*socket_number, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt fail");
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

static void decrypt(const char* key, char* buffer, int message_size)
{
	int key_size = strlen(key);
	int j = 0;

	for(int i = 0; i < message_size; i++)
	{
		*(buffer + i) -= key[j];
		j = (j + 1)%key_size;
	}	
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
        int receive_bytes  = recvfrom(socket_number, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&sin, &sin_len);
        if (receive_bytes < 0) 
        {
            perror("recvfrom fail");
            return FALSE;
        }

        buffer[receive_bytes] = '\0';
        decrypt("zaidenberg", buffer, strlen(buffer));
        puts(buffer);
    }

    return TRUE;
}
