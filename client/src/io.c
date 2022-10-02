#include "io.h"

int menu()
{
	printf(YELLOW);
	printf("\n--------------Yakonel chat--------------\n\n");
	printf(NORMAL);

	printf("1 - Registration\n");
	printf("2 - Log in\n");
    printf("3 - Create new group\n");
    printf("4 - Print existing groups\n");
    printf("5 - Join existing group\n");
    printf("6 - Leave group\n");
    printf("7 - Log out\n");
    printf("8 - Exit\n");
	printf( "Select your choice: ");

    int choice;
	scanf("%d", &choice);
	return choice;
}

void enter_user_name(char* user_name)
{
	printf("Enter user name: ");
	scanf("%s", user_name);  
}

void enter_group_name(char* group_name)
{
   	printf("Enter group name: ");
	scanf("%s", group_name);   
}

void enter_password(char* password)
{
    printf("Enter password: ");
	scanf("%s", password); 
}

static print_in_color_format(char* color, char* text)
{  
    printf(color);
	printf("\n  ->  ");
	printf("%s", text);
	printf("  <-  \n");
}

void print_invalid_choice()
{
    print_in_color_format(RED, "Invalid choice!");
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
            print_in_color_format(GREEN, "Successfully disconnected!");
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
            print_in_color_format(GREEN, "You left the group successfully!");
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
            print_invalid_choice();
            break;
    }
}

void print_groups_names_list(char* groups_names_list)
{
    printf("\nExisting groups:\n");
	printf("%s", groups_names_list);
}
