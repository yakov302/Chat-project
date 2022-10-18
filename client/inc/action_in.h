#ifndef ACTION_IN_H
#define ACTION_IN_H

#include "action_out.h"
#include "user.h"
#include "io.h"

#define GROUPS_PORT 5555
#define BUFFER_SIZE 1536
#define STRING_SIZE 68
#define FALSE 0
#define TRUE 1

typedef struct ActionIn
{
    int m_i_am_in_work;
    char* m_buffer;
    User* m_user;
    Mutex* m_mutex;
    Socket* m_socket;
    Message_type m_lest_message;

}ActionIn;

ActionIn* create_action_in(Socket* soket, Mutex* mutex, User* user);

void destroy_action_in(ActionIn* action_in);

void get_buffer(ActionIn* action_in, char* buffer);

int lest_message_arrive(ActionIn* action_in);


#endif // ACTION_IN_H
