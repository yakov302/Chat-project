# include "app.h"

static void* thread_function(void* arg)
{
	App* app = (App*)arg;
	run(app);
	return NULL;
}

App* app_create(User* user, Mutex* mutex, Socket* socket)
{
    if(user == NULL || mutex == NULL || socket == NULL)
        return NULL;

    App* app = (App*)malloc(sizeof(App));
    if(app == NULL)
        return NULL;

    app->m_stop = FALSE;
    app->m_user = user;
    app->m_mutex = mutex;
    app->m_socket = socket;
    app->m_thread_id = run_thread(thread_function, app);
    return app;
}

#define REGISTERATION 1
#define LOG_IN 2
#define CREATE_NEW_GROUP 3
#define PRINT_GROUPS_NAMES 4
#define JOIN_EXISTING_GROUP 5
#define LEAVE_GROUP 6
#define LOG_OUT 7
#define EXIT 8

void run(App* app)
{
    char user_name[STRING_SIZE];
    char group_name[STRING_SIZE];
    char password[STRING_SIZE];

    while (!app->m_stop)
    {
        int choice = menu(is_logged_in(app->m_user));
        switch (choice)
        {
            case REGISTERATION:
                enter_user_name(user_name);
                enter_password(password);
                send_requests_with_2_strings(user_name, password, REGISTRATION_REQUEST, app->m_socket, app->m_mutex);
                break;

            case LOG_IN:
                enter_user_name(user_name);
                enter_password(password);
                send_requests_with_2_strings(user_name, password, LOG_IN_REQUEST, app->m_socket, app->m_mutex);
                usleep(10000);
                break;

            case CREATE_NEW_GROUP:
                strcpy(user_name, name(app->m_user));
                enter_group_name(group_name);
                send_requests_with_2_strings(user_name, group_name, OPEN_NEW_GROUP_REQUEST, app->m_socket, app->m_mutex);
                break;

            case PRINT_GROUPS_NAMES:
                send_only_message(PRINT_EXISTING_GROUPS_REQUEST, app->m_socket, app->m_mutex);
                break;

            case JOIN_EXISTING_GROUP:
                strcpy(user_name, name(app->m_user));
                enter_group_name(group_name);
                send_requests_with_2_strings(user_name, group_name, JOIN_EXISTING_GROUP_REQUEST, app->m_socket, app->m_mutex);
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
                app_destroy(app);
                break;

            default:
                print_invalid_choice();
                break;
        }
    }
}

void app_destroy(App* app)
{
    app->m_stop = TRUE;
    join_thread(app->m_thread_id);
    free(app);
}