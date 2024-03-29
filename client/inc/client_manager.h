#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "app.h"

typedef struct ClientManager
{
    ActionIn* m_action_in;
    Socket* m_socket;
    Router* m_router;
    Mutex* m_mutex;
    User* m_user;
    App* m_app;

}ClientManager;

ClientManager* client_manager_create();

int is_client_manager_stop(ClientManager* client_manager);

void client_manager_destroy(ClientManager* client_manager);


#endif // CLIENT_MANAGER_H