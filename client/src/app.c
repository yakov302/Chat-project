# include "app.h"

static void* thread_function(void* arg)
{
	App* app = (App*)arg;
	run(app);
	return NULL;
}

App* app_create(User* user, Mutex* mutex,  Router* router, Socket* socket, ActionIn* action_in)
{
    if(user == NULL || mutex == NULL || socket == NULL)
        return NULL;

    App* app = (App*)malloc(sizeof(App));
    if(app == NULL)
        return NULL;

    app->m_stop = FALSE;
    app->m_user = user;
    app->m_mutex = mutex;
    app->m_router = router;
    app->m_socket = socket;
    app->m_action_in = action_in;
    app->m_thread_id = run_thread(thread_function, app);
    return app;
}

static void log_in(char* user_name, char* password, App* app, Socket *socket, Mutex *mutex)
{
    enter_user_name(user_name);
    enter_password(password);
    send_requests_with_2_strings(user_name, password, LOG_IN_REQUEST, app->m_socket, app->m_mutex);
}

static void join_exsisting_group(App* app, char* user_name, char* group_name)
{
    send_only_message(PRINT_EXISTING_GROUPS_REQUEST, app->m_socket, app->m_mutex);
    usleep(10000);

    if(app->m_action_in->m_lest_message != PRINT_EXISTING_GROUPS_NO_GROUPS)
    {
        strcpy(user_name, name(app->m_user));
        enter_group_name(group_name);
        send_requests_with_2_strings(user_name, group_name, JOIN_EXISTING_GROUP_REQUEST, app->m_socket, app->m_mutex);
    }
}

static void log_in_switch(App* app, int choice)
{
    char user_name[STRING_SIZE];
    char group_name[STRING_SIZE];

    switch (choice)
    {
        case CREATE_NEW_GROUP:
            strcpy(user_name, name(app->m_user));
            enter_group_name(group_name);
            send_requests_with_2_strings(user_name, group_name, OPEN_NEW_GROUP_REQUEST, app->m_socket, app->m_mutex);
            break;

        case JOIN_EXISTING_GROUP:
            join_exsisting_group(app, user_name, group_name);
            break;

        case LEAVE_GROUP:
            strcpy(user_name, name(app->m_user));
            enter_group_name(group_name);
            send_requests_with_2_strings(user_name, group_name, LEAVE_GROUP_REQUEST, app->m_socket, app->m_mutex);
            break;

        case LOG_OUT:
            strcpy(user_name, name(app->m_user));
            send_requests_with_1_strings(user_name, EXIT_CHAT_REQUEST, app->m_socket, app->m_mutex);
            break;

        case EXIT:
            print_exit();
            stop_router(app->m_router);
            app->m_stop = TRUE;
            break;

        default:
            print_invalid_choice();
            break;
    }
}

static void unlog_in_switch(App* app, int choice)
{
    char user_name[STRING_SIZE];
    char password[STRING_SIZE];

    switch (choice)
    {
        case REGISTERATION:
            enter_user_name(user_name);
            enter_password(password);
            send_requests_with_2_strings(user_name, password, REGISTRATION_REQUEST, app->m_socket, app->m_mutex);
            break;

        case LOG_IN:
            log_in(user_name, password, app, app->m_socket, app->m_mutex);
            break;

        case EXIT:
            print_exit();
            app->m_stop = TRUE;
            stop_router(app->m_router);
            break;

        default:
            print_invalid_choice();
            break;
    }
}

void run(App* app)
{
    char user_name[STRING_SIZE];
    char group_name[STRING_SIZE];
    char password[STRING_SIZE];

    while (!app->m_stop)
    {
        usleep(10000);
        int choice = menu(is_logged_in(app->m_user));
        
        if(is_logged_in(app->m_user))
            log_in_switch(app, choice);
        else
            unlog_in_switch(app, choice);
    }
}

void app_destroy(App* app)
{
    join_thread(app->m_thread_id);
    free(app);
}

int stop(App* app)
{
    return app->m_stop;
}