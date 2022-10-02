# include "app.h"

static void* thread_function(void* arg)
{
	App* app = (App*)arg;
	run_router(app);
	return NULL;
}

App* app_create(Mutex* mutex, Socket* socket)
{
    App* app = (App*)malloc(sizeof(App));
    if(app == NULL)
        return NULL;

    app->m_stop = FALSE;
    app->m_mutex = mutex;
    app->m_socket = socket;
    app->m_thread_id = run_thread(thread_function, app);
}

#define REGISTERATION 1
#define LOG_IN 2
#define CREATE_NEW_GROUP 3
#define PRINT_GROUPS_NAMES 4
#define JOIN_EXISTING_GROUP 5
#define LEAVE_GROUP 6
#define LOG_OUT 7
#define EXIT 8


    // REGISTRATION_REQUEST,
    // LOG_IN_REQUEST,
    // EXIT_CHAT_REQUEST,
    // OPEN_NEW_GROUP_REQUEST,
    // PRINT_EXISTING_GROUPS_REQUEST,
    // JOIN_EXISTING_GROUP_REQUEST,
    // LEAVE_GROUP_REQUEST,

void run(App* app)
{
    char name[STRING_SIZE];
    char password[STRING_SIZE];

    while (!app->m_stop)
    {
        int choice = menu();
        switch (choice)
        {
            case REGISTERATION:
                enter_user_name(name);
                enter_password(password);
                send_registration_or_log_in_request(name, password, REGISTRATION_REQUEST, app->m_socket, app->m_mutex);
                break;

            case LOG_IN:
                enter_user_name(name);
                enter_password(password);
                send_registration_or_log_in_request(name, password, LOG_IN_REQUEST, app->m_socket, app->m_mutex);
                break;

            case CREATE_NEW_GROUP:
                /* code */
                break;

            case PRINT_GROUPS_NAMES:
                /* code */
                break;

            case JOIN_EXISTING_GROUP:
                /* code */
                break;

            case LEAVE_GROUP:
                /* code */
                break;

            case LOG_OUT:
                /* code */
                break;

            case EXIT:
                /* code */
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