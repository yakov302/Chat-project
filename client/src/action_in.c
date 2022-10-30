#include "../inc/action_in.h"

ActionIn* create_action_in(Socket* soket, Mutex* mutex, User* user)
{
    if(soket == NULL || mutex == NULL || user == NULL)
        return NULL;

    ActionIn* action_in = (ActionIn*)malloc(sizeof(ActionIn));
    if(action_in == NULL)
        return NULL;

    action_in->m_buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    if(action_in->m_buffer == NULL)
    {
        free(action_in);
        return NULL;
    }

    action_in->m_user = user;
    action_in->m_mutex = mutex;
    action_in->m_socket = soket;
    return action_in;
}

void destroy_action_in(ActionIn* action_in)
{
    if(action_in == NULL)
        return;
    
    free(action_in->m_buffer);
    free(action_in);
    action_in = NULL;
}

static void give_1_strings(char* buffer, char* string)
{
    Args arg;
    args_create(&arg, 1, 0, 0);
    unpack(buffer, &arg);

    strcpy(string, strings_by_index(&arg, 0));
    args_destroy(&arg);
}

static void give_2_strings(char* buffer, char* first_string, char* second_string)
{
    Args arg;
    args_create(&arg, 2, 0, 0);
    unpack(buffer, &arg);

    strcpy(first_string, strings_by_index(&arg, 0));
    strcpy(second_string, strings_by_index(&arg, 1));

    args_destroy(&arg);
}

static void log_in_success(User* user, char* buffer)
{
    char name[STRING_SIZE];
    give_1_strings(buffer, name);
    set_name(user, name);
    set_log_in_status(user, TRUE);
    print_message(LOG_IN_SUCCESS, name);
}

static void log_out_success(User* user, char* buffer)
{
    remove_all_groups_from_user(user);
    set_log_in_status(user, FALSE);
    print_message(EXIT_CHAT_SUCCESS, user->m_name);
}

static void new_group(User* user, char* buffer, Message_type message_type)
{
    char group_name[STRING_SIZE];
    char group_ip[STRING_SIZE];
    give_2_strings(buffer, group_name, group_ip);

    Group* group = group_create(group_ip, GROUPS_PORT, group_name, name(user));
    add_group_for_user(user, group);
    print_message(message_type, group_name);
}

static void print_list_of_names(char* buffer)
{
    char list_of_names[BUFFER_SIZE];
    give_1_strings(buffer, list_of_names);
    print_buffer(list_of_names);
}

static void leave_group(User* user, char* buffer, Message_type message_type)
{
    char group_name[STRING_SIZE];
    give_1_strings(buffer, group_name);

    Group* group_ptr = group(user, group_name);
    remove_group_from_user(user, group_ptr);
    group_destroy(group_ptr);
    
    print_message(message_type, group_name);
}

void get_buffer(ActionIn* action_in, char* buffer)
{
    Message_type message = message_type(buffer);
    action_in->m_lest_message = message;
    char user_name[STRING_SIZE];
    char group_name[STRING_SIZE];

    switch (message)
    {
        case REGISTRATION_SUCCESS:
            give_1_strings(buffer, user_name);
            print_message(REGISTRATION_SUCCESS, user_name);
            break;

        case REGISTRATION_USER_NAME_ALREADY_EXISTS:
            give_1_strings(buffer, user_name);
            print_message(REGISTRATION_USER_NAME_ALREADY_EXISTS, user_name);
            break;

        case REGISTRATION_FAIL:
            print_message(REGISTRATION_FAIL, "");
            break; 

        case LOG_IN_SUCCESS:
            log_in_success(action_in->m_user, buffer);
            break;

        case LOG_IN_FAIL:
            print_message(LOG_IN_FAIL, "");
            break;

        case LOG_IN_WRONG_USER_NAME:
            print_message(LOG_IN_WRONG_USER_NAME, "");
            break;

        case LOG_IN_WRONG_PASSWORD:
            print_message(LOG_IN_WRONG_PASSWORD, "");
            break;

        case LOG_IN_USER_ALREADY_LOGGED_IN:
            give_1_strings(buffer, user_name);
            print_message(LOG_IN_USER_ALREADY_LOGGED_IN, user_name);
            break;

        case EXIT_CHAT_SUCCESS:
            log_out_success(action_in->m_user, buffer);
            break;

        case EXIT_CHAT_USER_NOT_EXISTS:
            print_message(EXIT_CHAT_USER_NOT_EXISTS, "");
            break;

        case EXIT_CHAT_FAIL:
            print_message(EXIT_CHAT_FAIL, "");
            break;

        case OPEN_NEW_GROUP_SUCCESS:
            new_group(action_in->m_user, buffer, OPEN_NEW_GROUP_SUCCESS);
            break;

        case OPEN_NEW_GROUP_FAIL:
            print_message(OPEN_NEW_GROUP_FAIL, "");
            break;

        case OPEN_NEW_GROUP_GROUP_NAME_ALREADY_EXISTS:
            give_1_strings(buffer, group_name);
            print_message(OPEN_NEW_GROUP_GROUP_NAME_ALREADY_EXISTS, group_name);
            break;

        case OPEN_NEW_GROUP_USER_NAME_NOT_EXISTS:
            print_message(OPEN_NEW_GROUP_USER_NAME_NOT_EXISTS, "");
            break;

        case PRINT_EXISTING_GROUPS_SUCCESS:
            print_list_of_names(buffer);
            break;

        case PRINT_EXISTING_GROUPS_NO_GROUPS:
            print_message(PRINT_EXISTING_GROUPS_NO_GROUPS, "");
            break;
        
        case PRINT_EXISTING_USERS_SUCCESS:
            print_list_of_names(buffer);
            break;

        case PRINT_EXISTING_USERS_NO_USERS:
            print_message(PRINT_EXISTING_USERS_NO_USERS, "");
            break;
        
        case JOIN_EXISTING_GROUP_SUCCESS:
            new_group(action_in->m_user, buffer, message);
            break;

        case JOIN_EXISTING_GROUP_FAIL:
            print_message(JOIN_EXISTING_GROUP_FAIL, "");
            break;

        case JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT:
            give_1_strings(buffer, group_name);
            print_message(JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT, group_name);
            break;

        case JOIN_EXISTING_GROUP_GROUP_NOT_EXISTS:
            print_message(JOIN_EXISTING_GROUP_GROUP_NOT_EXISTS, "");
            break;  

        case JOIN_EXISTING_GROUP_USER_NOT_EXISTS:
            print_message(JOIN_EXISTING_GROUP_USER_NOT_EXISTS, "");
            break;

        case LEAVE_GROUP_SUCCESS:
            leave_group(action_in->m_user, buffer, LEAVE_GROUP_SUCCESS);
            break;

        case LEAVE_GROUP_GROUP_DELETED:
            leave_group(action_in->m_user, buffer, LEAVE_GROUP_GROUP_DELETED);
            break;

        case LEAVE_GROUP_FAIL:
            print_message(LEAVE_GROUP_FAIL, "");
            break;

        case LEAVE_GROUP_GROUP_NOT_EXISTS:
            print_message(LEAVE_GROUP_GROUP_NOT_EXISTS, "");
            break;

        case LEAVE_GROUP_USER_NOT_EXISTS:
            print_message(LEAVE_GROUP_USER_NOT_EXISTS, "");
            break;
        
        case OPEN_PRIVATE_CHAT_SUCCESS:
            new_group(action_in->m_user, buffer, OPEN_PRIVATE_CHAT_SUCCESS);
            break;

        case OPEN_PRIVATE_CHAT_FAIL:
            print_message(OPEN_PRIVATE_CHAT_FAIL, "");
            break;

        case OPEN_PRIVATE_CHAT_PRIVATE_CHAT_ALREADY_EXISTS:
            print_message(OPEN_PRIVATE_CHAT_PRIVATE_CHAT_ALREADY_EXISTS, "");
            break;

        case OPEN_PRIVATE_CHAT_USER_NOT_EXISTS:
            print_message(OPEN_PRIVATE_CHAT_USER_NOT_EXISTS, "");
            break;
        
        case PING_SERVER_TO_CLIENT:
            send_only_message(PING_SERVER_TO_CLIENT, action_in->m_socket, action_in->m_mutex);
            break;

        default:
            break;
    }
}

int lest_message_arrive(ActionIn* action_in)
{
    if(action_in == NULL)
        return FALSE;

    return action_in->m_lest_message;
}

