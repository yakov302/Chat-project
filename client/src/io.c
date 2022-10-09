#include "io.h"

static void print_menu(int is_looged_in)
{
    printf(YELLOW);
	printf("\n--------------Yakonel chat--------------\n\n");
	printf(NORMAL);

    if(!is_looged_in)
    {
        printf(" 1 - Registration\n");
        printf(" 2 - Log in\n");
    }
    else
    {
        printf(" 3 - Create new group\n");
        printf(" 4 - Join existing group\n");
        printf(" 5 - Leave group\n");
        printf(" 6 - Log out\n");
    }

    printf(" 7 - Exit\n");
	printf("\nEnter your choice: ");
}

int menu(int is_looged_in)
{
    print_menu(is_looged_in);

    int choice = GET_OUT_FROM_SCANF;
	scanf("%d", &choice);
	return choice;
}

void enter_user_name(char* user_name)
{
    if(user_name == NULL)
    {
        printf("enter_user_name user_name not initialized\n");
        return;
    }

	printf("Enter user name: ");
	scanf("%s", user_name);  
}

void enter_group_name(char* group_name)
{
    if(group_name == NULL)
    {
        printf("enter_group_name group_name not initialized\n");
        return;
    }

   	printf("Enter group name: ");
	scanf("%s", group_name);   
}

void enter_password(char* password)
{
    if(password == NULL)
    {
        printf("enter_password password not initialized\n");
        return;
    }

    printf("Enter password: ");
	scanf("%s", password); 
}

static void print_in_color_format(char* color, char* text)
{  
    struct pollfd poll_stdin = {STDIN_FILENO, POLLIN|POLLPRI}; 
    poll(&poll_stdin, 1, 1);

    printf("%s", color);
	printf("\n                       ->  ");
	printf("%s", text);
	printf("  <-  \n");

    printf(NORMAL);
}

void print_invalid_choice()
{
    print_in_color_format(RED, "Invalid choice!");
}

void print_exit_chat()
{
    print_in_color_format(GREEN, "Successfully disconnected!\n");
}


void print_message(Message_type message_type)
{
    switch (message_type)
    {
        case REGISTRATION_SUCCESS:
            print_in_color_format(GREEN, "Registration success!");
            break;

        case REGISTRATION_USER_NAME_ALREADY_EXISTS:
            print_in_color_format(RED, "Username already taken!");
            break;

        case REGISTRATION_FAIL:
            print_in_color_format(RED, "Registration fail!");
            break; 

        case LOG_IN_SUCCESS:
            print_in_color_format(GREEN, "Log in success!");
            break;

        case LOG_IN_FAIL:
            print_in_color_format(RED, "Log in fail!");
            break;

        case LOG_IN_WRONG_USER_NAME:
            print_in_color_format(RED, "Wrong user name!");
            break;

        case LOG_IN_WRONG_PASSWORD:
            print_in_color_format(RED, "Wrong password!");
            break;

        case LOG_IN_USER_ALREADY_LOGGED_IN:
            print_in_color_format(RED, "You are already logged in!");
            break;

        case EXIT_CHAT_SUCCESS:
            print_in_color_format(GREEN, "Successfully loged out!");
            break;

        case EXIT_CHAT_USER_NOT_EXISTS:
            print_in_color_format(RED, "Fail. wrong user name!");
            break;

        case EXIT_CHAT_FAIL:
            print_in_color_format(RED, "Exit fail!");
            break;

        case OPEN_NEW_GROUP_SUCCESS:
            print_in_color_format(GREEN, "The group opened successfully!");
            break;

        case OPEN_NEW_GROUP_FAIL:
            print_in_color_format(RED, "Failed to open the group!");
            break;

        case OPEN_NEW_GROUP_GROUP_NAME_ALREADY_EXISTS:
            print_in_color_format(RED, "Group name already taken!");
            break;

        case OPEN_NEW_GROUP_USER_NAME_NOT_EXISTS:
            print_in_color_format(RED, "Fail. wrong user name!");
            break;

        case PRINT_EXISTING_GROUPS_SUCCESS:
            break;

        case PRINT_EXISTING_GROUPS_NO_GROUPS:
            print_in_color_format(RED, "No groups yet!");
            break;

        case JOIN_EXISTING_GROUP_SUCCESS:
            print_in_color_format(GREEN, "successfully connected to the group!");
            break;

        case JOIN_EXISTING_GROUP_FAIL:
            print_in_color_format(RED, "Join fail!");
            break;

        case JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT:
            print_in_color_format(RED, "You are already in the group!");
            break;

        case JOIN_EXISTING_GROUP_GROUP_NOT_EXISTS:
            print_in_color_format(RED, "Wrong group name!");
            break;

        case JOIN_EXISTING_GROUP_USER_NOT_EXISTS:
            print_in_color_format(RED, "Wrong user name!");
            break;

        case LEAVE_GROUP_SUCCESS:
            print_in_color_format(GREEN, "Successfully left the group!");
            break;

        case LEAVE_GROUP_GROUP_DELETED:
            print_in_color_format(GREEN, "You are lest group deleted!");
            break;

        case LEAVE_GROUP_FAIL:
            print_in_color_format(RED, "Live fail!");
            break;

        case LEAVE_GROUP_GROUP_NOT_EXISTS:
            print_in_color_format(RED, "Wrong group name!");
            break;

        case LEAVE_GROUP_USER_NOT_EXISTS:
            print_in_color_format(RED, "Wrong user name!");
            break;

        default:
            break;
    }
}

void print_groups_names_list(char* groups_names_list)
{
    if(groups_names_list == NULL)
    {
        printf("groups_names_list not initialized\n");
        return;
    }

    printf("\nExisting groups:\n");
	printf("%s\n", groups_names_list);
}
