#include "router.h"

#define TRUE 1

int main()
{
    Router* router = router_create(); 
    if(router == NULL)
    {
        printf("router not initialized\n");
        return 0;
    }

    while(TRUE)
    {
        char c = getchar();
        if(c == 'q' || c == 27)
        {
            stop_router(router);
            break;
        }
    }

    router_destroy(router);
}