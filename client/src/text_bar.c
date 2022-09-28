#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define MESSAGE_SIZE 320
#define TRUE 1
#define FALSE 0

static void save_process_id_to_file()
{	    	
	FILE* file = fopen("text_bar_process_id.txt", "w");
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

static int udp_client_init(int* socket_number, struct sockaddr_in* sin, char* ip, int port)
{
    *socket_number = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_number < 0) 
    {
        perror("open_socket failed!");
        return FALSE;
    }

    set_sin(sin, ip, port);
    return TRUE;
}

int main(int argc, char *argv[])
{
    if(argc < 4) 
    {printf("IP, PORT and USER NAME required\n"); return FALSE;}

    int socket_number;
    struct sockaddr_in sin;
    if(!udp_client_init(&socket_number, &sin, argv[1], atoi(argv[2])))
        return FALSE;

    char buffer[BUFFER_SIZE];
    char message[MESSAGE_SIZE];

    save_process_id_to_file();
    printf("Hello %s, type your message. \n", argv[3]);

    while (TRUE) 
    {
        fgets(buffer, BUFFER_SIZE, stdin);
        sprintf(message, "\033[1;32m%s\033[0m : %s", argv[3], buffer);

        int bytes_dend = sendto(socket_number, message, MESSAGE_SIZE, 0, (struct sockaddr*)&sin, sizeof(sin));
        if (bytes_dend < 0) 
        {
            sleep(5);
            perror("sendto fail!");
            return 1;
        }
        printf("sent\n");
    }

    return TRUE;
}