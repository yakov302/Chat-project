#ifndef ACTION_IN_H
#define ACTION_IN_H

#include "mutex.h"
#include "protocol.h"

typedef struct ActionIn
{


}ActionIn;

ActionIn* create_action_in(......);

void destroy_action_in(ActionIn* action_in);

void get_buffer(ActionIn* action_in, char* buffer, int client_socket, Mutex* mutex);


#endif // ACTION_IN_H
