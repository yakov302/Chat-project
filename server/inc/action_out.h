#ifndef ACTION_OUT_H
#define ACTION_OUT_H

#include "protocol.h"
#include "mutex.h"
#include "tcp.h"

#define BUFFER_SIZE 512
#define GRUPS_NAMES_LIST_SIZE 1536

void send_only_message(Message_type message_type, int client_socket, Mutex* mutex);

void send_connect_to_group_success(char* group_name, char* ip, Message_type message_type, int client_socket, Mutex* mutex);

void send_message_with_1_string(char* group_name, Message_type message_type, int client_socket, Mutex* mutex);


#endif // ACTION_OUT_H