#include "action_out.h"

void send_only_message(Message_type message_type, Socket* socket, Mutex* mutex)
{
    Args arg;
    char buffer[BUFFER_SIZE];
    args_create(&arg, 0, 0, 0);

    int size = pack(buffer, &arg, message_type);
    int result = send_to_server(socket, buffer, size, mutex);
    if(result != TRUE)
        printf("send to server fail, return value: %d\n", result);

    args_destroy(&arg);
}

static int pack_2_strings(char* buffer, Message_type message_type, char* first_string, char* second_string)
{
    Args arg;
    args_create(&arg, 2, 0, 0);
    push_string(&arg, first_string);
    push_string(&arg, second_string);

    int size = pack(buffer, &arg, message_type);

    args_destroy(&arg);
    return size;
}

void send_registration_or_log_in_request(char* user_name, char* password, Message_type message_type, Socket* socket, Mutex* mutex)
{
    char buffer[BUFFER_SIZE];
    int size = pack_2_strings(buffer, message_type, user_name, password);

    int result = send_to_server(socket, buffer, size, mutex);
    if(result != TRUE)
        printf("send to server fail, return value: %d\n", result);
}