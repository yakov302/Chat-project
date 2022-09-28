#ifndef ACTION_OUT_H
#define ACTION_OUT_H

#include "tcp.h"
#include "mutex.h"

void initial_notice(Socket* socket, char* buffer, Mutex* mutex);

#endif // ACTION_OUT_H