#include "action_out.h"

// There should be functions called from the application models

void initial_notice(Socket* socket, char* buffer, Mutex* mutex)
{
    char buff[512];
    char message[30];

    Args arg;
    args_create(&arg, 1, 1, 0);
    push_int(&arg, my_socket(socket));
    sprintf(message, "from client to server : %d", my_socket(socket));
    push_string(&arg, message);
    int size = pack(buff, &arg, WAKE_UP_CLIENT);
    args_destroy(&arg);

    send_to_server(socket, buff, size, mutex);
}
