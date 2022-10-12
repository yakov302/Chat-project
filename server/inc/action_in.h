#ifndef ACTION_IN_H
#define ACTION_IN_H

#include "action_out.h"
#include "groups_manager.h"
#include "subscribs_manager.h"
#include "users_manager.h"

#define LIST_OF_STRINGS_SIZE 1536

typedef struct ActionIn
{
    SubscribsManager* m_subscribs_manager;
    UsersManager* m_users_manager;
    GroupsManager* m_gruops_manager;

}ActionIn;

ActionIn* create_action_in(SubscribsManager* subscribs_manager, UsersManager* users_manager, GroupsManager* gruops_manager);

void destroy_action_in(ActionIn* action_in);

void get_buffer(ActionIn* action_in, char* buffer, int client_socket, Mutex* mutex);

void delete_disconnected_client(ActionIn* action_in, int client_socket);


#endif // ACTION_IN_H
