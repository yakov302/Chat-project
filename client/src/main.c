#include "group.h"
#include "user.h"
#include "tcp.h"
#include "router.h"
#include "list.h"
#include "app.h"
#include <string.h>
       #include <unistd.h>



#define TRUE 1

int main()
{
    // Router* router = router_create(); 
    // if(router == NULL)
    // {
    //     printf("router not initialized\n");
    //     return 0;
    // }

    // while(TRUE)
    // {
    //     char c = getchar();
    //     if(c == 'q' || c == 27)
    //     {
    //         stop_router(router);
    //         break;
    //     }
    // }

    // router_destroy(router);


    // Group* g =  group_create("224.255.255.1", 5555, "loby", y->m_name);
    // Group* g2 =  group_create("224.255.255.2", 5555, "loby-2", y->m_name);


    // add_group_for_user(y, g);
    // add_group_for_user(y, g2);

    // sleep(10);

    // Group* d = group(y, "loby-2");
    // remove_group_from_user(y, d);
    // group_destroy(d);

    User* y = create_user("hananel");
    if(y == NULL)
        printf("y\n");

    Socket* s = socket_create();
    if(s == NULL)
        printf("s\n");

    Mutex* m = tcp_create();
    if(m == NULL)
        printf("m\n");

    ActionIn* a = create_action_in(s, m, y);
    if(a == NULL)
        printf("A\n");

    Router* router = router_create(a, s, m); 
    if(router == NULL)
    {
        printf("router not initialized\n");
        return 0;
    }

    App* app = app_create(y, m, s);
    if(app == NULL)
        printf("app not iniy\n");

    while(TRUE)
    {
        // char c = getchar();
        // if(c == 'q' || c == 27)
        // {
        //     app_destroy(app);
        //     break;
        // }
    }
}
