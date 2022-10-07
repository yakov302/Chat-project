#ifndef APP_H
#define APP_H

#include "router.h"

#define STRING_SIZE 68
#define TRUE 1
#define FALSE 0

#define REGISTERATION 1
#define LOG_IN 2
#define CREATE_NEW_GROUP 3
#define JOIN_EXISTING_GROUP 4
#define LEAVE_GROUP 5
#define LOG_OUT 6
#define EXIT 7

typedef struct App
{
    int m_stop;
    int m_thread_id;

    User* m_user;
    Mutex* m_mutex;
    Router* m_router;
    Socket* m_socket;
    ActionIn* m_action_in;

}App;

App* app_create(User* user, Mutex* mutex, Router* router, Socket* socket, ActionIn* action_in);

void run_app(App* app);

int is_app_stop(App* app);

void app_destroy(App* group);


#endif // APP_H
