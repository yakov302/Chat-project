#ifndef TCP_H
#define TCP_H

#include <errno.h>

#include "socket.h"
#include "mutex.h"
#include "protocol.h"

#define UNINITIALIZED_ARGS -1
#define ACCEPT_FAIL -2
#define MUTEX_FAIL -3
#define RECEIVE_SUCCESS 1
#define RECEIVE_FAIL 0

Mutex* tcp_create();

void tcp_destroy(Mutex* mutex);

int receive_from_server(Socket* socket,char* buffe, Mutex* mutex);

int send_to_server(Socket* socket,char* buffer, int message_size, Mutex* mutex);


#endif //TCP_H
