#include "thread.h" 

#define NUM_OF_THREADS 30
#define FAIL -1
#define USED 1
#define UNUSED 0

pthread_t thread[NUM_OF_THREADS];
char status[NUM_OF_THREADS] = {0};

int find_id()
{
    for(int i = 0; i < NUM_OF_THREADS; ++i)
    {
        if(status[i] == UNUSED)
            return i;
    }
    
    return FAIL;
}

int run_thread(ThreadFunc function, void* arg)
{
    if(function == NULL)
    {
        printf("%s", "Need to passed ThreadFunc to the function.\n");
        return FAIL;
    }

    int i = find_id();
    if(i == FAIL)
    {
        printf("%s", "No pthread_t available. increase NUM_OF_THREADS.\n");
        return FAIL;
    }

    if(pthread_create(&thread[i], NULL, function, arg) != 0)
        perror("pthread_create fail");
    
    status[i] = USED;
    return i;
}

void join_thread(int thread_id)
{
    if(pthread_join(thread[thread_id], NULL) != 0)
        perror("pthread_join fail");
    
    status[thread_id] = UNUSED;
}
