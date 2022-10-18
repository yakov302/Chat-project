#ifndef APP_H
#define APP_H

#include "router.h"

#define PROCESS_KILLED -1
#define STRING_SIZE 68
#define FALSE 0
#define TRUE 1
#define ESC 27

#define REGISTERATION 1
#define LOG_IN 2
#define CREATE_NEW_GROUP 3
#define JOIN_EXISTING_GROUP 4
#define LEAVE_GROUP 5
#define CREATE_PRIVATE_CHAT 6
#define LOG_OUT 7
#define EXIT 8

typedef struct App
{
    int m_stop;
    int m_run_app_thread_id;
    int m_check_close_groups_thread_id;


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
