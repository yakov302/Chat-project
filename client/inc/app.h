#ifndef APP_H
#define APP_H

#include "action_out.h"
#include "socket.h"
#include "thread.h"
#include "mutex.h"
#include "user_c.h"
#include "io.h"

#define STRING_SIZE 68
#define TRUE 1
#define FALSE 0
#define REGISTERATION 1
#define LOG_IN 2
#define CREATE_NEW_GROUP 3
#define PRINT_GROUPS_NAMES 4
#define JOIN_EXISTING_GROUP 5
#define LEAVE_GROUP 6
#define LOG_OUT 7
#define EXIT 8

typedef struct App
{
    int m_stop;
    int m_thread_id;

    User* m_user;
    Mutex* m_mutex;
    Socket* m_socket;

}App;

App* app_create(User* user, Mutex* mutex, Socket* socket);

void run(App* app);

void app_destroy(App* group);


#endif // APP_H
