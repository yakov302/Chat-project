# include "app.h"

static void* thread_function(void* arg)
{
	App* app = (App*)arg;
	run_app(app);
	return NULL;
}

App* app_create(User* user, Mutex* mutex,  Router* router, Socket* socket, ActionIn* action_in)
{
    if(user == NULL || mutex == NULL || router == NULL || socket == NULL || action_in == NULL)
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

static void registration(App* app, char* user_name, char* password)
{
    enter_user_name(user_name);
    enter_password(password);
    send_requests_with_2_strings(user_name, password, REGISTRATION_REQUEST, app->m_socket, app->m_mutex);
}

static void log_in(App* app, Socket *socket, Mutex *mutex, char* user_name, char* password)
{
    enter_user_name(user_name);
    enter_password(password);
    send_requests_with_2_strings(user_name, password, LOG_IN_REQUEST, app->m_socket, app->m_mutex);
}

static void create_new_group(App* app, char* user_name, char* group_name)
{
    strcpy(user_name, name(app->m_user));
    enter_group_name(group_name);
    send_requests_with_2_strings(user_name, group_name, OPEN_NEW_GROUP_REQUEST, app->m_socket, app->m_mutex);
}

static void join_exsisting_group(App* app, char* user_name, char* group_name)
{
    send_only_message(PRINT_EXISTING_GROUPS_REQUEST, app->m_socket, app->m_mutex);
    usleep(10000);

    if(lest_message_arrive(app->m_action_in) != PRINT_EXISTING_GROUPS_NO_GROUPS)
    {
        strcpy(user_name, name(app->m_user));
        enter_group_name(group_name);
        send_requests_with_2_strings(user_name, group_name, JOIN_EXISTING_GROUP_REQUEST, app->m_socket, app->m_mutex);
    }
}

static void leave_group(App* app, char* user_name, char* group_name)
{
    strcpy(user_name, name(app->m_user));
    enter_group_name(group_name);
    send_requests_with_2_strings(user_name, group_name, LEAVE_GROUP_REQUEST, app->m_socket, app->m_mutex);
}

static void log_out(App* app, char* user_name)
{
    strcpy(user_name, name(app->m_user));
    send_requests_with_1_strings(user_name, EXIT_CHAT_REQUEST, app->m_socket, app->m_mutex);
}

static void exit_chat(App* app)
{
    stop_router(app->m_router);
    app->m_stop = TRUE;
    print_exit_chat(); 
}

static void logged_switch(App* app, int choice)
{
    char user_name[STRING_SIZE];
    char group_name[STRING_SIZE];

    switch (choice)
    {
        case CREATE_NEW_GROUP:
            create_new_group(app, user_name, group_name);
            break;

        case JOIN_EXISTING_GROUP:
            join_exsisting_group(app, user_name, group_name);
            break;

        case LEAVE_GROUP:
            leave_group(app, user_name, group_name);
            break;

        case LOG_OUT:
            log_out(app, user_name);
            break;

        case EXIT:
            log_out(app, user_name);
            exit_chat(app);
            break;

        default:
            print_invalid_choice();
            break;
    }
}

static void unlogged_switch(App* app, int choice)
{
    char user_name[STRING_SIZE];
    char password[STRING_SIZE];

    switch (choice)
    {
        case REGISTERATION:
            registration(app, user_name, password);
            break;

        case LOG_IN:
            log_in(app, app->m_socket, app->m_mutex, user_name, password);
            break;

        case EXIT:
            exit_chat(app);
            break;

        default:
            print_invalid_choice();
            break;
    }
}

void run_app(App* app)
{
    while (!app->m_stop)
    {
        usleep(10000);
        int choice = menu(is_logged_in(app->m_user));
        
        if(is_logged_in(app->m_user))
            logged_switch(app, choice);
        else
            unlogged_switch(app, choice);
    }
}

void app_destroy(App* app)
{
    join_thread(app->m_thread_id);
    free(app);
}

int is_app_stop(App* app)
{
    return app->m_stop;
}