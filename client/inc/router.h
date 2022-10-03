#ifndef ROUTER_H
#define ROUTER_H

#include <sys/select.h>

#include "action_in.h"
#include "action_out.h"
#include "thread.h"
#include "socket.h"
#include "tcp.h"

#define MAX_SOCKET_AMOUNT_TO_LISTEN 1024
#define BUFFER_SIZE 1536
#define OFF 0
#define ON 1

typedef struct Router
{
    ActionIn* m_action_in;
    Socket* m_socket;
    Mutex* m_mutex;

    int m_stop;
    char* m_buffer;
    int m_thread_id;

}Router;

Router* router_create(ActionIn* action_in, Socket* socket, Mutex* mutex);

void router_destroy(Router* router);

void run_router(Router* router);

void stop_router(Router* router);


#endif //ROUTER_H
