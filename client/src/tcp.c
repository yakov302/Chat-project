#include "../inc/tcp.h"

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

int receive_from_server(Socket* socket, char* buffer, Mutex* mutex)
{
    if(buffer == NULL || mutex == NULL) {return UNINITIALIZED_ARGS;}
	if(!mutex_lock(mutex)) {return MUTEX_FAIL;}
	
	int receive_bytes = recv(my_socket(socket), buffer, sizeof(int), 0);
	if(receive_bytes == 0) {perror("receive fail: \n"); mutex_unlock(mutex); return RECEIVE_FAIL;}

	int message_len = message_size(buffer);
	while(receive_bytes < message_len)
	{
		int current_receive = recv(my_socket(socket), (buffer + receive_bytes), message_len - receive_bytes, 0);
		if(current_receive == 0) {perror("receive fail"); mutex_unlock(mutex); return RECEIVE_FAIL;}
		receive_bytes += current_receive;
	}	

	if(!mutex_unlock(mutex)) {return MUTEX_FAIL;}
	return RECEIVE_SUCCESS;
}

int send_to_server(Socket* socket, char* buffer, int message_size, Mutex* mutex)
{
    if(buffer == NULL || mutex == NULL) {return UNINITIALIZED_ARGS;}
    if(!mutex_lock(mutex)) {return MUTEX_FAIL;}

	int sent_byte = 0;
	while((sent_byte < message_size) && (errno != EPIPE))
	{
    	int current_byte = send(my_socket(socket), (buffer + sent_byte), (message_size - sent_byte), 0);
		if(current_byte < 0){perror("send fail");}
		sent_byte += current_byte;
	}

    if(!mutex_unlock(mutex)){return MUTEX_FAIL;}
	if(errno == EPIPE){return FALSE;}
	return TRUE;
}

