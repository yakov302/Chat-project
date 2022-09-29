#include "group.h"
#include "user.h"
#include "list.h"
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

    User* y = create_user("hananel");

    Group* g =  group_create("224.255.255.1", 5555, "loby", y->m_name);
    Group* g2 =  group_create("224.255.255.2", 5555, "loby-2", y->m_name);


    add_group_for_user(y, g);
    add_group_for_user(y, g2);

    sleep(10);

    Group* d = group(y, "loby-2");
    remove_group_from_user(y, d);
    group_destroy(d);
}