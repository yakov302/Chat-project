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

static int exit_to_menu(App* app, char* string)
{
    if(*(char*)string == ESC)
    {
        set_work_status(app->m_action_in, FALSE);
        return TRUE;
    }
    return FALSE;
}

static void registration_or_log_in(App* app, char* user_name, char* password, Message_type message_type)
{
    enter_user_name(user_name);
    if(exit_to_menu(app, user_name))
        return;

    enter_password(password);
    if(exit_to_menu(app, password))
        return;

    send_requests_with_2_strings(user_name, password, message_type, app->m_socket, app->m_mutex);
}

static void create_group(App* app, char* group_or_user_name, int is_private, Message_type message_type)
{
    if(is_private)
    {
        enter_user_name(group_or_user_name);
        if(exit_to_menu(app, group_or_user_name))
            return;
    }
    else
    {
        enter_group_name(group_or_user_name);
        if(exit_to_menu(app, group_or_user_name))
            return;
    }

    send_requests_with_2_strings_and_1_int(name(app->m_user), group_or_user_name, is_private, message_type, app->m_socket, app->m_mutex);
}

static void join_or_leave_group(App* app, char* group_name, Message_type message_type)
{
    enter_group_name(group_name);
    if(exit_to_menu(app, group_name))
        return;

    send_requests_with_2_strings(name(app->m_user), group_name, message_type, app->m_socket, app->m_mutex);
}

static void join_exsisting_group(App* app, char* group_name)
{
    send_only_message(PRINT_EXISTING_GROUPS_REQUEST, app->m_socket, app->m_mutex);
    usleep(10000);

    if(lest_message_arrive(app->m_action_in) != PRINT_EXISTING_GROUPS_NO_GROUPS)
        join_or_leave_group(app, group_name, JOIN_EXISTING_GROUP_REQUEST);
}

static void create_private_chat(App* app, char* group_name)
{
    send_only_message(PRINT_EXISTING_USERS_REQUEST, app->m_socket, app->m_mutex);
    usleep(10000);

    if(lest_message_arrive(app->m_action_in) != PRINT_EXISTING_USERS_NO_USERS)
        create_group(app, group_name, TRUE, OPEN_PRIVATE_CHAT_REQUEST);
}


static void log_out(App* app)
{
    send_requests_with_1_strings(name(app->m_user), EXIT_CHAT_REQUEST, app->m_socket, app->m_mutex);
}

static void exit_chat(App* app)
{
    stop_router(app->m_router);
    app->m_stop = TRUE;
    usleep(10000);
    print_exit_chat(name(app->m_user)); 
}

static void logged_switch(App* app, int choice)
{
    char group_name[STRING_SIZE];
    char user_name[STRING_SIZE];

    switch (choice)
    {
        case CREATE_NEW_GROUP:
            create_group(app, group_name, FALSE, OPEN_NEW_GROUP_REQUEST);
            break;
        
        case JOIN_EXISTING_GROUP:
            join_exsisting_group(app, group_name);
            break;

        case LEAVE_GROUP:
            join_or_leave_group(app, group_name, LEAVE_GROUP_REQUEST);
            break;

        case CREATE_PRIVATE_CHAT:
            create_private_chat(app, group_name);
            break;

        case LOG_OUT:
            log_out(app);
            break;

        case EXIT:
            log_out(app);
            exit_chat(app);
            break;

        case GET_OUT_FROM_SCANF:
            break;

        default:
            set_work_status(app->m_action_in, FALSE);
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
            registration_or_log_in(app, user_name, password, REGISTRATION_REQUEST);
            break;

        case LOG_IN:
            registration_or_log_in(app, user_name, password, LOG_IN_REQUEST);
            break;

        case EXIT:
            exit_chat(app);
            break;

        case GET_OUT_FROM_SCANF:
            break;

        default:
            print_invalid_choice();
            set_work_status(app->m_action_in, FALSE);
            break;
    }
}

static int close_kill_groups(void* group, void* app)
{
    if((Group*)group == NULL)
        return TRUE;

    if(kill(((Group*)group)->m_chat_window_process_id, 0) == PROCESS_KILLED
    || kill(((Group*)group)->m_text_bar_process_id, 0) == PROCESS_KILLED)
    {
        printf("in app.c -> close_kill_groups() -> kill %s group\n", ((Group*)group)->m_name);
        send_requests_with_2_strings(name(((App*)app)->m_user), ((Group*)group)->m_name, LEAVE_GROUP_REQUEST, ((App*)app)->m_socket, ((App*)app)->m_mutex);
    }

    return TRUE;
}

static void check_if_groups_are_live(App* app)
{
    list_for_each(groups_list(app->m_user), close_kill_groups, app);
}

void run_app(App* app)
{
    while (!app->m_stop)
    {
        //usleep(100000);
        while(work_in_is_working(app->m_action_in)){usleep(10000);}

        int choice = menu(is_logged_in(app->m_user));

        set_work_status(app->m_action_in, TRUE);
        
        if(is_logged_in(app->m_user))
            logged_switch(app, choice);
        else
            unlogged_switch(app, choice);

        check_if_groups_are_live(app);
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