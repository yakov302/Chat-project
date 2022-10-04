#ifndef ACTION_IN_H
#define ACTION_IN_H

#include "mutex.h"
#include "user.h"
#include "protocol.h"
#include "action_out.h"
#include "socket.h"
#include "io.h"

#define GROUPS_PORT 5555
#define BUFFER_SIZE 1536
#define TRUE 1

typedef struct ActionIn
{
    User* m_user;
    Mutex* m_mutex;
    Socket* m_socket;
    char* m_buffer;

}ActionIn;

ActionIn* create_action_in(Socket* soket, Mutex* mutex, User* user);

void destroy_action_in(ActionIn* action_in);

void get_buffer(ActionIn* action_in, char* buffer);


#endif // ACTION_IN_H
