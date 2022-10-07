#include "client_manager.h"

int main()
{
    ClientManager* client_manager = client_manager_create();
    if(client_manager == NULL)
    {
        printf("client manager not initialized\n");
        return 0;
    }

    while(TRUE)
    {
        if(is_client_manager_stop(client_manager))
            break;

    }

    client_manager_destroy(client_manager);
}
