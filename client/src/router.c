#include "router.h"

static void* thread_function(void* arg)
{
	Router* router = (Router*)arg;
	run_router(router);
	return NULL;
}

Router* router_create()
{
    Router* router = (Router*)malloc(sizeof(Router));
    if(router == NULL)
        return NULL;
    
    router->m_socket = socket_create();
    if(router->m_socket == NULL)
    {
        free(router);
        return NULL;
    }

    router->m_mutex = tcp_create();
    if(router->m_mutex == NULL)
    {
        socket_destroy(router->m_socket);
        free(router);
        return NULL;
    }

    router->m_buffer = (char*)malloc(BUFFER_SIZE*sizeof(char));
    if(router->m_socket == NULL)
    {
        free(router->m_mutex);
        socket_destroy(router->m_socket);
        free(router);
        return NULL;
    }

    router->m_stop = OFF;
    router->m_thread_id = run_thread(thread_function, (void*)router);
    return router;
}

void router_destroy(Router* router)
{
    if(router == NULL)
        return;

    join_thread(router->m_thread_id);
    free(router->m_buffer);
    tcp_destroy(router->m_mutex);
    socket_destroy(router->m_socket);
    free(router);
    router = NULL;

    printf("+ router destroyed\n");
}

static void fatal_error(Router* router, const char* fail)
{
	perror(fail);
    stop_router(router);
    router_destroy(router);
}

void run_router(Router* router)
{
    printf("+ router up\n");
    initial_notice(router->m_socket, router->m_buffer, router->m_mutex); // should be deleted
    
    while(!router->m_stop) 
	{
        printf("+ enter select\n");	

		int activity = select(MAX_SOCKET_AMOUNT_TO_LISTEN, fd(router->m_socket), NULL, NULL, NULL);
		if((activity < 0) && (errno != EINTR))
			fatal_error(router, "select fail: \n");

        printf("+ exit select\n");	

        if(!receive_from_server(router->m_socket, router->m_buffer, router->m_mutex))
			fatal_error(router, "receive fail \n");

        get_buffer(router->m_socket, router->m_buffer, router->m_mutex);
	}

    printf("+ router down\n");
}

static void wake_up(Router* router)
{
    Args arg;
    char buff[40];

    args_create(&arg, 0, 0, 0);
    int size = pack(buff, &arg, WAKE_UP_CLIENT);
    args_destroy(&arg);

    send_to_server(router->m_socket, buff, size, router->m_mutex);
}

void stop_router(Router* router)
{
    router->m_stop = ON;
   // wake_up(router);
}