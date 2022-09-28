#ifndef ACTION_IN_H
#define ACTION_IN_H

#include "tcp.h"
#include "mutex.h"

void get_buffer(Socket* socket, char* buffer, Mutex* mutex);


#endif // ACTION_IN_H
