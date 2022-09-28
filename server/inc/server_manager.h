#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include "router.h"
#include "action_in.h"
#include "action_out.h"
#include "users_manager.h"
#include "groups_manager.h"
#include "subscribs_manager.h"

typedef struct ServerManager
{
    Router* m_router;
    ActionIn* m_action_in;
    UsersManager* m_users_manager;
    GroupsManager* m_groups_manager;
    SubscribsManager* m_subscribs_manager;

}ServerManager;

ServerManager* server_manager_create(int max_num_of_groups, int max_num_of_users, int max_num_of_subscribs);

void server_manager_destroy(ServerManager* server_manager);


#endif // SERVER_MANAGER_H
