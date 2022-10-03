#include "action_in.h"

ActionIn* create_action_in(User* user)
{
    if(user == NULL)
        return NULL;

    ActionIn* action_in = (ActionIn*)malloc(sizeof(ActionIn));
    if(action_in == NULL)
        return NULL;

    action_in->m_buffer = (char*)malloc(sizeof(char)* BUFFER_SIZE);
    if(action_in->m_buffer == NULL)
    {
        free(action_in);
        return NULL;
    }
    
    action_in->m_user = user;
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
    print_message(LOG_IN_SUCCESS);
    char name[STRING_SIZE];
    give_1_strings(buffer, name);
    set_name(user, name);
    set_is_logged_in(user, TRUE);
}

static void new_group(User* user, char* buffer)
{
    print_message(OPEN_NEW_GROUP_SUCCESS);
    char group_name[STRING_SIZE];
    char group_ip[STRING_SIZE];
    give_2_strings(buffer, group_name, group_ip);

    Group* group = group_create(group_ip, GROUPS_PORT, group_name, name(user));
    add_group_for_user(user, group);
}

void get_buffer(ActionIn* action_in, char* buffer)
{
    Message_type type = message_type(buffer);

    switch (type)
    {
        case REGISTRATION_SUCCESS:
            print_message(REGISTRATION_SUCCESS);
            break;

        case REGISTRATION_USER_NAME_ALREADY_EXISTS:
            print_message(REGISTRATION_USER_NAME_ALREADY_EXISTS);
            break;

        case REGISTRATION_FAIL:
            print_message(REGISTRATION_FAIL);
            break; 

        case LOG_IN_SUCCESS:
            log_in_success(action_in->m_user, buffer);
            break;

        case LOG_IN_FAIL:
            print_message(LOG_IN_FAIL);
            break;

        case LOG_IN_WRONG_USER_NAME:
            print_message(LOG_IN_WRONG_USER_NAME);
            break;

        case LOG_IN_WRONG_PASSWORD:
            print_message(LOG_IN_WRONG_PASSWORD);
            break;

        case LOG_IN_USER_ALREADY_LOGGED_IN:
            print_message(LOG_IN_USER_ALREADY_LOGGED_IN);
            break;

        case EXIT_CHAT_SUCCESS:
            print_message(EXIT_CHAT_SUCCESS);
            break;

        case EXIT_CHAT_USER_NOT_EXISTS:
            print_message(EXIT_CHAT_USER_NOT_EXISTS);
            break;

        case EXIT_CHAT_FAIL:
            print_message(EXIT_CHAT_FAIL);
            break;

        case OPEN_NEW_GROUP_SUCCESS:
            new_group(action_in->m_user, buffer);
            break;

        case OPEN_NEW_GROUP_FAIL:

            break;

        case OPEN_NEW_GROUP_GROUP_NAME_ALREADY_EXISTS:

            break;

        case OPEN_NEW_GROUP_USER_NAME_NOT_EXISTS:

            break;

        case PRINT_EXISTING_GROUPS_SUCCESS:

            break;

        case PRINT_EXISTING_GROUPS_NO_GROUPS:

            break;

        case JOIN_EXISTING_GROUP_SUCCESS:

            break;

        case JOIN_EXISTING_GROUP_FAIL:

            break;

        case JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT:

            break;

        case JOIN_EXISTING_GROUP_GROUP_NOT_EXISTS:

            break;

        case JOIN_EXISTING_GROUP_USER_NOT_EXISTS:

            break;

        case LEAVE_GROUP_SUCCESS:

            break;

        case LEAVE_GROUP_FAIL:

            break;

        case LEAVE_GROUP_GROUP_NOT_EXISTS:

            break;

        case LEAVE_GROUP_USER_NOT_EXISTS:

            break;

        default:

            break;
    }
}

