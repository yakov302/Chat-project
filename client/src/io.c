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
        printf(" 6 - Create private chat\n");
        printf(" 7 - Log out\n");
    }

    printf(" 8 - Exit\n");
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

	printf("Enter user name (Esc for back): ");
	scanf("%s", user_name);  
}

void enter_group_name(char* group_name)
{
    if(group_name == NULL)
    {
        printf("enter_group_name group_name not initialized\n");
        return;
    }

   	printf("Enter group name (Esc for back): ");
	scanf("%s", group_name);
}

void enter_password(char* password)
{
    if(password == NULL)
    {
        printf("enter_password password not initialized\n");
        return;
    }

    printf("Enter password (Esc for back): ");
	scanf("%s", password); 
}


static void print_in_color_format(char* color, char* text)
{  
    char windwo_size[8];
    int string_size = strlen(text)*1.3;
    sprintf(windwo_size, "%dx5", string_size);

    char command[COMMAND_SIZE];
    sprintf(command, "gnome-terminal -q --geometry=%s --title=pop_up_message -- ./pop_up_message %s %s",windwo_size, color, text); 
           
    int result = system(command);
    if(result == ERROR) {perror("system fail!");}
}

void print_invalid_choice()
{
    print_in_color_format(RED, "Invalid choice!");
}

static void concat_string_at_the_beginning(char* string, char* text, char* concat_message)
{
    strcat(concat_message, string);
    strcat(concat_message, text);
}

static void concat_string_at_the_end(char* string, char* text, char* concat_message)
{
    strcat(concat_message, text);
    strcat(concat_message, string);
    strcat(concat_message, " group!");
}

void print_exit_chat(char* name)
{
    char concat_message[MESSAGE_SIZE];
    concat_string_at_the_beginning(name, " disconnected!", concat_message);
    print_in_color_format(GREEN, concat_message);
}

void print_message(Message_type message_type, char* string)
{
    char concat_message[MESSAGE_SIZE] = {0};

    switch (message_type)
    {
        case REGISTRATION_SUCCESS:
            concat_string_at_the_beginning(string, " has successfully registered!", concat_message);
            print_in_color_format(GREEN, concat_message);
            break;

        case REGISTRATION_USER_NAME_ALREADY_EXISTS:
            concat_string_at_the_beginning(string, " already taken!", concat_message);
            print_in_color_format(RED, concat_message);
            break;

        case REGISTRATION_FAIL:
            print_in_color_format(RED, "Registration fail!");
            break; 

        case LOG_IN_SUCCESS:
            concat_string_at_the_beginning(string, " login successfully!", concat_message);
            print_in_color_format(GREEN, concat_message);
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
            concat_string_at_the_beginning(string,  " already logged in!", concat_message);
            print_in_color_format(RED, concat_message);
            break;

        case EXIT_CHAT_SUCCESS:
            concat_string_at_the_beginning(string, " logout successfully!", concat_message);
            print_in_color_format(GREEN, concat_message);
            break;

        case EXIT_CHAT_USER_NOT_EXISTS:
            print_in_color_format(RED, "Wrong user name!");
            break;

        case EXIT_CHAT_FAIL:
            print_in_color_format(RED, "Log out fail!");
            break;

        case OPEN_NEW_GROUP_SUCCESS:
            concat_string_at_the_beginning(string, " group opened successfully!",  concat_message);
            print_in_color_format(GREEN, concat_message);
            break;

        case OPEN_NEW_GROUP_FAIL:
            print_in_color_format(RED, "Open group fail!");
            break;

        case OPEN_NEW_GROUP_GROUP_NAME_ALREADY_EXISTS:
            concat_string_at_the_beginning(string, " group already exists!", concat_message);
            print_in_color_format(RED, concat_message);
            break;

        case OPEN_NEW_GROUP_USER_NAME_NOT_EXISTS:
            print_in_color_format(RED, "Wrong user name!");
            break;

        case PRINT_EXISTING_GROUPS_SUCCESS:
            break;

        case PRINT_EXISTING_GROUPS_NO_GROUPS:
            print_in_color_format(RED, "No groups yet!");
            break;

        case PRINT_EXISTING_USERS_SUCCESS:
            break;

        case PRINT_EXISTING_USERS_NO_USERS:
            print_in_color_format(RED, "No users connected!");
            break;

        case JOIN_EXISTING_GROUP_SUCCESS:
            concat_string_at_the_end(string, "Successfully join ", concat_message);
            print_in_color_format(GREEN, concat_message);
            break;

        case JOIN_EXISTING_GROUP_FAIL:
            print_in_color_format(RED, "Join fail!");
            break;

        case JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT:
            concat_string_at_the_end(string, "You lready in ", concat_message);
            print_in_color_format(RED, concat_message);
            break;

        case JOIN_EXISTING_GROUP_GROUP_NOT_EXISTS:
            print_in_color_format(RED, "Wrong group name!");
            break;

        case JOIN_EXISTING_GROUP_USER_NOT_EXISTS:
            print_in_color_format(RED, "Wrong user name!");
            break;

        case LEAVE_GROUP_SUCCESS:
            concat_string_at_the_end(string, "Successfully left ", concat_message);
            print_in_color_format(GREEN, concat_message);
            break;

        case LEAVE_GROUP_GROUP_DELETED:
            concat_string_at_the_end(string, "Successfully left ", concat_message);
            print_in_color_format(GREEN, concat_message);
            print_in_color_format(GREEN, "You are last - group deleted!");
            break;

        case LEAVE_GROUP_FAIL:
            print_in_color_format(RED, "Live group fail!");
            break;

        case LEAVE_GROUP_GROUP_NOT_EXISTS:
            print_in_color_format(RED, "Wrong group name!");
            break;

        case LEAVE_GROUP_USER_NOT_EXISTS:
            print_in_color_format(RED, "Wrong user name!");
            break;

        case OPEN_PRIVATE_CHAT_SUCCESS:
            concat_string_at_the_beginning(string, " room opened successfully!",  concat_message);
            print_in_color_format(GREEN, concat_message);
            break;

        case OPEN_PRIVATE_CHAT_FAIL:
            print_in_color_format(RED, "Open private chat fail!");
            break;

        case OPEN_PRIVATE_CHAT_PRIVATE_CHAT_ALREADY_EXISTS:
            concat_string_at_the_beginning(string, " room already exists!", concat_message);
            print_in_color_format(RED, concat_message);
            break;

        case OPEN_PRIVATE_CHAT_USER_NOT_EXISTS:
            print_in_color_format(RED, "Wrong user name!");
            break;

        default:
            break;
    }
}

void print_buffer(char* names_list)
{
    if(names_list == NULL)
    {
        printf("names_list not initialized\n");
        return;
    }

	printf("%s\n", names_list);
}
