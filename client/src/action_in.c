#include "action_in.h"

void get_buffer(ActionIn* action_in, char* buffer, int client_socket, Mutex* mutex)
{
    Message_type type = message_type(buffer);

    switch (type)
    {
        case REGISTRATION_SUCCESS:

            break;

        case REGISTRATION_USER_NAME_ALREADY_EXISTS:

            break;

        case REGISTRATION_FAIL:

            break; 

        case LOG_IN_SUCCESS:

            break;

        case LOG_IN_FAIL:

            break;

        case LOG_IN_WRONG_USER_NAME:

            break;

        case LOG_IN_WRONG_PASSWORD:

            break;

        case LOG_IN_USER_ALREADY_LOGGED_IN:

            break;

        case EXIT_CHAT_SUCCESS:

            break;

        case EXIT_CHAT_USER_NOT_EXISTS:

            break;

        case EXIT_CHAT_FAIL:

            break;

        case OPEN_NEW_GROUP_SUCCESS:

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

