#include "tcp.h"

Mutex* tcp_create()
{
    Mutex* mutex = (Mutex*)malloc(sizeof(Mutex));
    mutex_create(mutex);
    return mutex;
}

void tcp_destroy(Mutex* mutex)
{
    if(mutex == NULL)
        return;

    mutex_destroy(mutex);
    free(mutex);
    mutex = NULL;
}

int accept_new_client(Socket* socket, Mutex* mutex)
{
    if(socket == NULL || mutex == NULL) {return UNINITIALIZED_ARGS;}    
	if(!mutex_lock(mutex)) {return MUTEX_FAIL;}

	socklen_t sin_len = sizeof(client_sin(socket));
	int client_socket = accept(listen_socket(socket), client_sin(socket), &sin_len);
	if(client_socket < 0) {mutex_unlock(mutex); return ACCEPT_FAIL;}

    printf("+ accept client: %d\n", client_socket);
	if(!mutex_unlock(mutex)){return MUTEX_FAIL;}
	return client_socket;
}

int receive_from_client(int client_socket, char* buffer, Mutex* mutex)
{
    if(buffer == NULL || mutex == NULL) {return UNINITIALIZED_ARGS;}
	if(!mutex_lock(mutex)) {return MUTEX_FAIL;}
	
	int receive_bytes = recv(client_socket, buffer, sizeof(int), 0);
	if(receive_bytes == 0) {perror("receive fail: \n"); mutex_unlock(mutex); return RECEIVE_FAIL;}

	int message_len = message_size(buffer);
	while(receive_bytes < message_len)
	{
		int current_receive = recv(client_socket, (buffer + receive_bytes), message_len - receive_bytes, 0);
		if(current_receive == 0) {perror("receive fail: \n"); mutex_unlock(mutex); return RECEIVE_FAIL;}
		receive_bytes += current_receive;
	}	

    printf("+ recv from client: %d\n", client_socket);
	if(!mutex_unlock(mutex)) {return MUTEX_FAIL;}
	return RECEIVE_SUCCESS;
}

int send_to_client(int client_socket, char* buffer, int message_size, Mutex* mutex)
{
    if(buffer == NULL || mutex == NULL) {return UNINITIALIZED_ARGS;}
    if(!mutex_lock(mutex)) {return MUTEX_FAIL;}

	int sent_byte = 0;
	while((sent_byte < message_size) && (errno != EPIPE))
	{
    	int current_byte = send(client_socket, (buffer + sent_byte), (message_size - sent_byte), 0);
		if(current_byte < 0){perror("send fail: \n");}
		sent_byte += current_byte;
	}

	signal(SIGPIPE, SIG_IGN);
    if(!mutex_unlock(mutex)){return MUTEX_FAIL;}
	if(errno == EPIPE){return FALSE;}
    printf("+ send to client: %d\n", client_socket);
	return TRUE;
}

void send_all_clients(Socket* socket, char* buffer, int message_size, Mutex* mutex)
{
    if(socket == NULL || buffer == NULL)
        return;

    ListItr next_it;
    ListItr it = begin(connected_sockets(socket));
	ListItr end_it = end(connected_sockets(socket));
	while(it != end_it) 
	{	
        next_it = next(it);

        int client_socket = *(int*)get_data(it);
		if(!send_to_client(client_socket, buffer, message_size, mutex))
			delete_client(socket, it);
        
        it = next_it;
	} 
}
