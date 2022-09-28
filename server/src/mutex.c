#include "mutex.h"

int mutex_create(Mutex* mutex)
{
    if(mutex == NULL)
        return MUTEX_NOT_INITIALIZE;

    if(pthread_mutex_init(&mutex->m_lock, NULL) != 0)
    {
        perror("mutex_create fail: \n");
        return FAIL;
    }
    return SUCCESS;
}

int mutex_destroy(Mutex* mutex)
{
    if(mutex == NULL)
        return MUTEX_NOT_INITIALIZE;

    if(pthread_mutex_destroy(&mutex->m_lock) != 0)
    {
        perror("mutex_destroy fail: \n");
        return FAIL;
    }
    return SUCCESS;
}

int mutex_lock(Mutex* mutex)
{
    if(mutex == NULL)
        return MUTEX_NOT_INITIALIZE;

    if(pthread_mutex_lock(&mutex->m_lock) != 0)
    {
        perror("mutex_lock fail: \n");
        return FAIL;
    }
    return SUCCESS;
}

int mutex_unlock(Mutex* mutex)
{
    if(mutex == NULL)
        return MUTEX_NOT_INITIALIZE;

    if(pthread_mutex_unlock(&mutex->m_lock) != 0)
    {
        perror("mutex_lock fail: \n");
        return FAIL;
    }
    return SUCCESS;
}
