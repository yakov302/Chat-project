#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <stdio.h>

#define MUTEX_NOT_INITIALIZE -1
#define SUCCESS 1
#define FAIL 0

typedef struct Mutex
{
    pthread_mutex_t m_lock;

}Mutex;

int mutex_create(Mutex* mutex);

int mutex_destroy(Mutex* mutex);

int mutex_lock(Mutex* mutex);

int mutex_unlock(Mutex* mutex);


#endif // MUTEX_H
