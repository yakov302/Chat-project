#include "client_manager.h"

ClientManager* client_manager_create()
{
    ClientManager* client_manager = (ClientManager*)malloc(sizeof(ClientManager));
    if(client_manager == NULL)
        return NULL;

    client_manager->m_user = create_user("");
    if(client_manager->m_user == NULL)
    {
        free(client_manager);
        return NULL;
    }

    client_manager->m_socket = socket_create();
    if(client_manager->m_socket == NULL)
    {
        destroy_user(client_manager->m_user);
        free(client_manager);
        return NULL;
    }

    client_manager->m_mutex = tcp_create();
    if(client_manager->m_mutex == NULL)
    {
        socket_destroy(client_manager->m_socket);
        destroy_user(client_manager->m_user);
        free(client_manager);
        return NULL;
    }

    client_manager->m_action_in = create_action_in(client_manager->m_socket, client_manager->m_mutex, client_manager->m_user);
    if(client_manager->m_action_in == NULL)
    {
        tcp_destroy(client_manager->m_mutex);
        socket_destroy(client_manager->m_socket);
        destroy_user(client_manager->m_user);
        free(client_manager);
        return NULL;
    }

    client_manager->m_router = router_create(client_manager->m_action_in, client_manager->m_socket, client_manager->m_mutex); 
    if(client_manager->m_router == NULL)
    {
        destroy_action_in(client_manager->m_action_in);
        tcp_destroy(client_manager->m_mutex);
        socket_destroy(client_manager->m_socket);
        destroy_user(client_manager->m_user);
        free(client_manager);
        return NULL;
    }

    client_manager->m_app = app_create(client_manager->m_user, client_manager->m_mutex, client_manager->m_router, client_manager->m_socket, client_manager->m_action_in);
    if(client_manager->m_app == NULL)
    {
        router_destroy(client_manager->m_router);
        destroy_action_in(client_manager->m_action_in);
        tcp_destroy(client_manager->m_mutex);
        socket_destroy(client_manager->m_socket);
        destroy_user(client_manager->m_user);
        free(client_manager);
        return NULL;
    }

    return client_manager;
}

int is_client_manager_stop(ClientManager* client_manager)
{
    return is_app_stop(client_manager->m_app);
}

void client_manager_destroy(ClientManager* client_manager)
{
    if(client_manager == NULL)
        return;

    app_destroy(client_manager->m_app);
    router_destroy(client_manager->m_router);
    destroy_action_in(client_manager->m_action_in);
    tcp_destroy(client_manager->m_mutex);
    socket_destroy(client_manager->m_socket);
    destroy_user(client_manager->m_user);
    free(client_manager);
    client_manager = NULL;
}