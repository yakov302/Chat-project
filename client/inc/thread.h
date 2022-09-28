#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>

typedef void*(*ThreadFunc)(void*);

int run_thread(ThreadFunc function, void* arg);

void join_thread(int thread_id);


#endif // THREAD_H