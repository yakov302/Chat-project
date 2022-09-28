#include "server_manager.h"

ServerManager* server_manager_create(int max_num_of_groups, int max_num_of_users,int max_num_of_subscribs)
{
    ServerManager* server_manager = (ServerManager*)malloc(sizeof(ServerManager));
    if(server_manager == NULL) {return NULL;}
    
    server_manager->m_groups_manager = create_groups_manager(max_num_of_groups);
    if(server_manager->m_groups_manager == NULL) 
    {
        free(server_manager);
        return NULL;
    }

    server_manager->m_users_manager = create_users_manager(max_num_of_users);
    if(server_manager->m_users_manager == NULL)
    {
        destroy_groups_manager(server_manager->m_groups_manager);
        free(server_manager);
        return NULL;
    }

    server_manager->m_subscribs_manager = create_subscribs_manager(max_num_of_subscribs);
    if(server_manager->m_subscribs_manager == NULL)
    {
        destroy_users_manager(server_manager->m_users_manager);
        destroy_groups_manager(server_manager->m_groups_manager);
        free(server_manager);
        return NULL;
    }

    server_manager->m_action_in = create_action_in(server_manager->m_subscribs_manager, server_manager->m_users_manager, server_manager->m_groups_manager);
    if(server_manager->m_action_in == NULL)
    {
        destroy_subscribs_manager(server_manager->m_subscribs_manager);
        destroy_users_manager(server_manager->m_users_manager);
        destroy_groups_manager(server_manager->m_groups_manager);
        free(server_manager);
        return NULL;
    }

    server_manager->m_router = router_create(server_manager->m_action_in);
    if(server_manager->m_router == NULL)
    {
        destroy_action_in(server_manager->m_action_in);
        destroy_subscribs_manager(server_manager->m_subscribs_manager);
        destroy_users_manager(server_manager->m_users_manager);
        destroy_groups_manager(server_manager->m_groups_manager);
        free(server_manager);
        return NULL;
    }

    return server_manager;
}

void server_manager_destroy(ServerManager* server_manager)
{
    if(server_manager == NULL)
        return;

    stop_router(server_manager->m_router);
    router_destroy(server_manager->m_router);
    destroy_action_in(server_manager->m_action_in);
    destroy_subscribs_manager(server_manager->m_subscribs_manager);
    destroy_users_manager(server_manager->m_users_manager);
    destroy_groups_manager(server_manager->m_groups_manager);
    free(server_manager);
    server_manager = NULL;
}