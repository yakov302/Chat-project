#include "../inc/action_out.h"

void send_only_message(Message_type message_type, Socket* socket, Mutex* mutex)
{
    Args arg;
    char buffer[BUFFER_OUT_SIZE];
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

static int pack_2_strings_and_1_int(char* buffer, Message_type message_type, char* first_string, char* second_string, int integer)
{
    Args arg;
    args_create(&arg, 2, 1, 0);
    push_string(&arg, first_string);
    push_string(&arg, second_string);
    push_int(&arg, integer);

    int size = pack(buffer, &arg, message_type);

    args_destroy(&arg);
    return size;
}

static int pack_1_strings(char* buffer, Message_type message_type, char* string)
{
    Args arg;
    args_create(&arg, 1, 0, 0);
    push_string(&arg, string);

    int size = pack(buffer, &arg, message_type);

    args_destroy(&arg);
    return size;
}

void send_requests_with_2_strings(char* first_string, char* second_string, Message_type message_type, Socket* socket, Mutex* mutex)
{
    char buffer[BUFFER_OUT_SIZE];
    int size = pack_2_strings(buffer, message_type, first_string, second_string);

    int result = send_to_server(socket, buffer, size, mutex);
    if(result != TRUE)
        printf("send to server fail, return value: %d\n", result);
}

void send_requests_with_2_strings_and_1_int(char* first_string, char* second_string, int integer, Message_type message_type, Socket* socket, Mutex* mutex)
{
    char buffer[BUFFER_OUT_SIZE];
    int size = pack_2_strings_and_1_int(buffer, message_type, first_string, second_string, integer);

    int result = send_to_server(socket, buffer, size, mutex);
    if(result != TRUE)
        printf("send to server fail, return value: %d\n", result);
}

void send_requests_with_1_strings(char* string, Message_type message_type, Socket* socket, Mutex* mutex)
{
    char buffer[BUFFER_OUT_SIZE];
    int size = pack_1_strings(buffer, message_type, string);

    int result = send_to_server(socket, buffer, size, mutex);
    if(result != TRUE)
        printf("send to server fail, return value: %d\n", result); 
}
