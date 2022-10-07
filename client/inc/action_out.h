#ifndef ACTION_OUT_H
#define ACTION_OUT_H

#include "tcp.h"

#define BUFFER_OUT_SIZE 512

void send_only_message(Message_type message_type, Socket* socket, Mutex* mutex);

void send_requests_with_2_strings(char* first_string, char* second_string, Message_type message_type, Socket* socket, Mutex* mutex);

void send_requests_with_1_strings(char* string, Message_type message_type, Socket* socket, Mutex* mutex);


#endif // ACTION_OUT_H