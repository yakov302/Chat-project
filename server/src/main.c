#include "server_manager.h"

int main()
{
    ServerManager* sm =  server_manager_create(250, 300, 1000);
    if(sm == NULL)
    {
        printf("server manager not initialized\n");
        return 0;
    }

    while(TRUE)
    {
        char c = getchar();
        if(c == 'q' || c == 27)
        {
            server_manager_destroy(sm);
            break;
        }
    }
}