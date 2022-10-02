#ifndef ACTION_OUT_H
#define ACTION_OUT_H

#include "tcp.h"
#include "mutex.h"

#define BUFFER_SIZE 512

void send_only_message(Message_type message_type, Socket* socket, Mutex* mutex);

void send_registration_or_log_in_request(char* user_name, char* password, Message_type message_type, Socket* socket, Mutex* mutex);


#endif // ACTION_OUT_H