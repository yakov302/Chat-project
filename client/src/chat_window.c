#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

#define MAX_SOCKET_AMOUNT_TO_LISTEN 10
#define PROCESS_KILLED -1
#define SELECT_FAIL -1
#define BUFFER_SIZE 1024
#define IP_SIZE 20
#define TRUE 1
#define FALSE 0

static void save_process_id_to_file()
{	 
    pid_t process_id;   	
	FILE* file = fopen("../resources/chat_window_process_id.txt", "w");
	if(file == NULL) {printf("fopen fail!\n"); return;}

    process_id = getpid();
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

static void set_multicast(struct ip_mreq* mreq, char* ip)
{
    memset(mreq, 0, sizeof(*mreq));
    mreq->imr_multiaddr.s_addr = inet_addr(ip);
    mreq->imr_interface.s_addr = htonl(INADDR_ANY);
}

static int udp_server_init(int* socket_number, struct sockaddr_in* sin, char* ip, int port)
{
    *socket_number = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_number < 0) 
    {perror("open_socket failed!"); return FALSE;}

    int optval = 1;
    if (setsockopt(*socket_number, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {perror("setsockopt fail"); return FALSE;}

    struct ip_mreq mreq;
    set_multicast(&mreq, ip);
    if (setsockopt(*socket_number, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {perror("setsockopt fail"); return FALSE;}

    set_sin(sin, ip, port);
    if(bind(*socket_number, (struct sockaddr*)sin, sizeof(*sin)) < 0) 
    {perror("bind fail!"); return FALSE;}

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

static void set_fd(int socket_number, fd_set* fd)
{
    FD_ZERO(fd);
    FD_SET(socket_number, fd);
}

static int enter_time_out_select(int socket_number, fd_set* fd)
{
    set_fd(socket_number, fd);
    struct timeval timeval = {3, 0};
    int activity = select(MAX_SOCKET_AMOUNT_TO_LISTEN, fd, NULL, NULL, &timeval);
    if((activity < 0) && (errno != EINTR))
        return SELECT_FAIL;

    return activity;
}

static void check_if_main_process_still_alive(pid_t main_process_id, pid_t text_bar_process_id, int* run_loop)
{
    if(kill(main_process_id, 0) == PROCESS_KILLED)
    {
        kill(text_bar_process_id, SIGKILL);
        *run_loop = FALSE;
    }
}

static void fatal_error(char* explain, int* run_loop)
{
    perror(explain);
    *run_loop = FALSE;
}

int main(int argc, char* argv[])
{
    save_process_id_to_file();

    if(argc < 5) 
    {printf("IP, PORT, MAIN PROCESS ID and TEXT BAR PROCESS ID required\n"); return FALSE;}
    
    pid_t main_process_id = atoi(argv[3]);
    pid_t text_bar_process_id = atoi(argv[4]);

    int socket_number;
    struct sockaddr_in sin;
    if(!udp_server_init(&socket_number, &sin, argv[1], atoi(argv[2])))
    {printf("udp_server_init fail!\n"); return FALSE;}

    int run_loop = TRUE;
    char buffer[BUFFER_SIZE];
    socklen_t sin_len = sizeof(sin);
    fd_set* fd = (fd_set*)malloc(sizeof(*fd));

    while(run_loop) 
    {
        int result = enter_time_out_select(socket_number, fd);
        if(result)
        {
            int receive_bytes  = recvfrom(socket_number, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&sin, &sin_len);
            if (receive_bytes < 0) {fatal_error("recvfrom fail", &run_loop);}

            buffer[receive_bytes] = '\0';
            decrypt("zaidenberg", buffer, strlen(buffer));
            puts(buffer);
        }
        else if(result == SELECT_FAIL) {fatal_error("select fail", &run_loop);}

        check_if_main_process_still_alive(main_process_id, text_bar_process_id, &run_loop);
    }

    free(fd);
    return TRUE;
}
