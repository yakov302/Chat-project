#include "action_in.h"

ActionIn* create_action_in(SubscribsManager* subscribs_manager, UsersManager* users_manager, GroupsManager* gruops_manager)
{
    if(subscribs_manager == NULL || users_manager == NULL || gruops_manager == NULL) 
        return NULL;
    
    ActionIn* action_in = (ActionIn*)malloc(sizeof(ActionIn));
    if(action_in == NULL) {return NULL;}

    action_in->m_subscribs_manager = subscribs_manager;
    action_in->m_users_manager = users_manager;
    action_in->m_gruops_manager = gruops_manager;
    return action_in;
}

void destroy_action_in(ActionIn* action_in)
{
    if(action_in == NULL)
        return;
    
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

static void give_2_strings_and_1_int(char* buffer, char* first_string, char* second_string, int* integer)
{
    Args arg;
    args_create(&arg, 2, 1, 0);
    unpack(buffer, &arg);

    strcpy(first_string, strings_by_index(&arg, 0));
    strcpy(second_string, strings_by_index(&arg, 1));
    *integer = ints(&arg)[0];

    args_destroy(&arg);
}

static void registeration_request(SubscribsManager* subscribs_manager, char* buffer, int client_socket, Mutex* mutex)
{
    char name[STRING_SIZE]; char password[STRING_SIZE];
    give_2_strings(buffer, name, password);

    SubscribsManager_return result = new_subscriber(subscribs_manager, name, password);
    switch (result)
    {
        case SUBSCRIBS_MANAGER_USERNAME_ALREADY_EXISTS:
            send_message_with_1_string(name, REGISTRATION_USER_NAME_ALREADY_EXISTS, client_socket, mutex);
            break;

        case SUBSCRIBS_MANAGER_SUCCESS:
            send_message_with_1_string(name, REGISTRATION_SUCCESS, client_socket, mutex);
            break;    

        default:
            printf("new_subscriber fail! SubscribsManager_return: %d\n", result);
            send_only_message(REGISTRATION_FAIL, client_socket, mutex);
            break;
    }
}

static void log_in_request(SubscribsManager* subscribs_manager, UsersManager* users_manager, char* buffer, int client_socket, Mutex* mutex)
{
    char name[STRING_SIZE]; char password[STRING_SIZE];
    give_2_strings(buffer, name, password);

    SubscribsManager_return s_result = log_in_chack(subscribs_manager, name, password);
    switch (s_result)
    {
        case SUBSCRIBS_MANAGER_USERNAME_NOT_EXISTS:
            send_only_message(LOG_IN_WRONG_USER_NAME, client_socket, mutex);
            return;

        case SUBSCRIBS_MANAGER_INVALID_PASSWORD:
            send_only_message(LOG_IN_WRONG_PASSWORD, client_socket, mutex);
            return;

        case SUBSCRIBS_MANAGER_SUCCESS:
            break;

        default:
            printf("log_in_chack fail! SubscribsManager_return: %d\n", s_result);
            send_only_message(LOG_IN_FAIL, client_socket, mutex);
            return;
    }

    UsersManager_return u_result = user_log_in(users_manager, name, client_socket);
    switch (u_result)
    {
        case USER_MANAGER_USER_ALREADY_LOGGED_IN:
            send_message_with_1_string(name, LOG_IN_USER_ALREADY_LOGGED_IN, client_socket, mutex);
            break;

        case USER_MANAGER_SUCCESS:
            send_message_with_1_string(name, LOG_IN_SUCCESS, client_socket, mutex);
            break;
        
        default:
            printf("user_log_in fail! UsersManager_return: %d\n", u_result);
            send_only_message(LOG_IN_FAIL, client_socket, mutex);
            break;
    }
}

static void leave_chat_request(UsersManager* users_manager, GroupsManager* groups_manager, char* buffer, int client_socket, Mutex* mutex)
{
    char name[STRING_SIZE]; 
    give_1_strings(buffer, name);

    GroupsManager_return g_result = leave_all_groups(groups_manager, user_groups_list(users_manager, name), name);
    switch (g_result)
    {
        case GROUPS_MANAGER_SUCCESS:
            break;
            
        default:
            printf("leave_all_groups fail! GroupsManager_return: %d\n", g_result);
            send_only_message(EXIT_CHAT_FAIL, client_socket, mutex);
            return;
    }

    UsersManager_return result = user_log_out(users_manager, name);
    switch (result)
    {
        case USER_MANAGER_USER_NOT_EXISTS:
            send_only_message(EXIT_CHAT_USER_NOT_EXISTS, client_socket, mutex);
            break;

         case USER_MANAGER_SUCCESS:
            send_message_with_1_string(name, EXIT_CHAT_SUCCESS, client_socket, mutex);
            break;

        default:
            printf("user_log_out fail! UsersManager_return: %d\n", result);
            send_only_message(EXIT_CHAT_FAIL, client_socket, mutex);
            break;
    }
}

static int leave_group_action(GroupsManager* groups_manager, UsersManager* users_manager, char* user_name, char* group_name, int client_socket, Mutex* mutex)
{
    UsersManager_return u_result = user_leave_group(users_manager, user_name, group_name);
    switch (u_result)
    {
        case USER_MANAGER_USER_NOT_EXISTS:
            send_only_message(LEAVE_GROUP_USER_NOT_EXISTS, client_socket, mutex);
            return GROUP_LIVE;

         case USER_MANAGER_GROUP_NOT_EXISTS:
            send_message_with_1_string(group_name, LEAVE_GROUP_GROUP_NOT_EXISTS, client_socket, mutex);
            return GROUP_LIVE;

         case USER_MANAGER_SUCCESS:
            break;

        default:
            printf("user_leave_group fail! UsersManager_return: %d\n", u_result);
            send_only_message(LEAVE_GROUP_FAIL, client_socket, mutex);
            return GROUP_LIVE;
    }

    GroupsManager_return g_result = leave_group(groups_manager, group_name, user_name);
    switch (g_result)
    {
        case GROUPS_MANAGER_GROUP_NOT_EXISTS:
            send_only_message(LEAVE_GROUP_GROUP_NOT_EXISTS, client_socket, mutex);
            user_join_group(users_manager, user_name, group_name);
            break;

        case GROUPS_MANAGER_GROUP_DELETED:
            send_message_with_1_string(group_name, LEAVE_GROUP_GROUP_DELETED, client_socket, mutex);
            return GROUP_DEAD;

        case GROUPS_MANAGER_USER_NOT_EXISTS:
            send_only_message(LEAVE_GROUP_USER_NOT_EXISTS, client_socket, mutex);

        case GROUPS_MANAGER_SUCCESS:
            send_message_with_1_string(group_name, LEAVE_GROUP_SUCCESS, client_socket, mutex);
            break;
        
        default:
            printf("leave_group fail! GroupsManager_return: %d\n", g_result);
            user_join_group(users_manager, user_name, group_name);
            send_only_message(LEAVE_GROUP_FAIL, client_socket, mutex);
            break;
    }

    return GROUP_LIVE;
}

static int leave_private_chat(void* hash_element, void* args)
{
	if(leave_group_action(((ArgsForLeavePrivateChat*)args)->groups_manager, 
                          ((ArgsForLeavePrivateChat*)args)->users_manager, 
                          ((Element*)hash_element)->m_key, 
                          ((ArgsForLeavePrivateChat*)args)->group_name, 
                          user_socket(((ArgsForLeavePrivateChat*)args)->users_manager, ((Element*)hash_element)->m_key), 
                          ((ArgsForLeavePrivateChat*)args)->mutex)){return FALSE;}
    return TRUE;
}

static ArgsForLeavePrivateChat* make_args(GroupsManager* groups_manager, UsersManager* users_manager, char* group_name, Mutex* mutex)
{
    ArgsForLeavePrivateChat* args = (ArgsForLeavePrivateChat*)malloc(sizeof(ArgsForLeavePrivateChat));
    args->groups_manager = groups_manager;
    args->users_manager = users_manager;
    strcpy(args->group_name, group_name);
    args->mutex = mutex;
    return args;
}

static void leave_group_request(GroupsManager* groups_manager, UsersManager* users_manager, char* buffer, int client_socket, Mutex* mutex)
{
    char user_name[STRING_SIZE]; char group_name[STRING_SIZE];
    give_2_strings(buffer, user_name, group_name);

    if(is_a_private(groups_manager, group_name))
    {
        ArgsForLeavePrivateChat* args = make_args(groups_manager, users_manager, group_name, mutex);
        HashMap* users = group_users(groups_manager, group_name);
        hash_map_for_each(users, leave_private_chat, (void*)args);
        free(args);
    }
    else
        leave_group_action(groups_manager, users_manager, user_name, group_name, client_socket, mutex);
}

static void open_group_action(GroupsManager* groups_manager, UsersManager* users_manager, Mutex* mutex, char* user_name, char* group_name, char* ip,  int is_private, int client_socket)
{
    GroupsManager_return g_result = new_group(groups_manager, group_name, user_name, ip, is_private);
    switch (g_result)
    {
        case GROUPS_MANAGER_GROUPNAME_ALREADY_EXISTS:
            send_message_with_1_string(group_name, OPEN_NEW_GROUP_GROUP_NAME_ALREADY_EXISTS, client_socket, mutex);
            return;

        case GROUPS_MANAGER_SUCCESS:
            break;   
            
        default:
            printf("new_group fail! GroupsManager_return: %d\n", g_result);
            send_only_message(OPEN_NEW_GROUP_FAIL, client_socket, mutex);
            return;
    }

    UsersManager_return u_result = user_join_group(users_manager, user_name, group_name);
    switch (u_result)
    {
        case USER_MANAGER_USER_NOT_EXISTS:
            leave_group(groups_manager, group_name, user_name);
            send_only_message(OPEN_NEW_GROUP_USER_NAME_NOT_EXISTS, client_socket, mutex);
            break;

        case USER_MANAGER_SUCCESS:
            send_connect_to_group_success(group_name, ip, OPEN_NEW_GROUP_SUCCESS, client_socket, mutex);
            break;   
            
        default:
            printf("user_join_group fail! UsersManager_return: %d\n", u_result);
            leave_group(groups_manager, group_name, user_name);
            send_only_message(OPEN_NEW_GROUP_FAIL, client_socket, mutex);
            break;
    }
}

static void set_group_name(char* group_name, char* user_name_1, char* user_name_2)
{
    sprintf(group_name, "\"Private %s & %s\"", user_name_1, user_name_2);
}

static int open_private_chat(GroupsManager* groups_manager, UsersManager* users_manager, Mutex* mutex, char* user_name, char* group_name, char* ip,  int is_private, int client_socket)
{
    GroupsManager_return g_result = new_group(groups_manager, group_name, user_name, ip, is_private);
    switch (g_result)
    {
        case GROUP_MANAGER_USER_ALREADY_IN_GROUP:
            send_message_with_1_string(group_name, OPEN_PRIVATE_CHAT_PRIVATE_CHAT_ALREADY_EXISTS, client_socket, mutex);

        case GROUPS_MANAGER_SUCCESS:
            break;   
            
        default:
            printf("new_group fail! GroupsManager_return: %d\n", g_result);
            send_only_message(OPEN_PRIVATE_CHAT_FAIL, client_socket, mutex);
            return FALSE;
    }

    UsersManager_return u_result = user_join_group(users_manager, user_name, group_name);
    switch (u_result)
    {
        case USER_MANAGER_USER_ALREADY_IN_THE_GROUP:
            send_message_with_1_string(group_name, OPEN_PRIVATE_CHAT_PRIVATE_CHAT_ALREADY_EXISTS, client_socket, mutex);
            break;

        case USER_MANAGER_USER_NOT_EXISTS:
            leave_group(groups_manager, group_name, user_name);
            send_only_message(OPEN_PRIVATE_CHAT_USER_NOT_EXISTS, client_socket, mutex);
            break;

        case USER_MANAGER_SUCCESS:
            send_connect_to_group_success(group_name, ip, OPEN_PRIVATE_CHAT_SUCCESS, client_socket, mutex);
            return TRUE;  
            
        default:
            printf("user_join_group fail! UsersManager_return: %d\n", u_result);
            leave_group(groups_manager, group_name, user_name);
            send_only_message(OPEN_PRIVATE_CHAT_FAIL, client_socket, mutex);
            break;
    }

    return FALSE;
}

static void open_private_chat_action(GroupsManager* groups_manager, UsersManager* users_manager, Mutex* mutex, char* user_name_1, char* user_name_2, char* ip,  int is_private, int client_socket)
{
    if(!user_log_in_check(users_manager, user_name_2))
    {
        send_only_message(OPEN_PRIVATE_CHAT_USER_NOT_EXISTS, client_socket, mutex);
        return;
    }

    char group_name[STRING_SIZE];
    set_group_name(group_name, user_name_1, user_name_2);

   if(open_private_chat(groups_manager, users_manager, mutex, user_name_1, group_name, ip, is_private, client_socket))
   {
        usleep(200000); //relevant only when you run multy clients from the same computer
        if(!open_private_chat(groups_manager, users_manager, mutex, user_name_2, group_name, ip, is_private, user_socket(users_manager, user_name_2)))
            leave_group_action(groups_manager, users_manager, user_name_1, group_name, client_socket, mutex);
   }
}

static void open_group_request(GroupsManager* groups_manager, UsersManager* users_manager, char* buffer, int client_socket, Mutex* mutex)
{
    char user_name[STRING_SIZE]; char group_or_user_name[STRING_SIZE]; char return_ip[IP_SIZE]; int is_private;
    give_2_strings_and_1_int(buffer, user_name, group_or_user_name, &is_private);

    if(!is_private)
        open_group_action(groups_manager, users_manager, mutex, user_name, group_or_user_name, return_ip, is_private, client_socket);
    else
        open_private_chat_action(groups_manager, users_manager, mutex, user_name, group_or_user_name, return_ip, is_private, client_socket);
}

static void print_existing_groups_request(GroupsManager* groups_manager, int client_socket, Mutex* mutex)
{
    if(num_of_groups(groups_manager) < 1)
    {
        send_only_message(PRINT_EXISTING_GROUPS_NO_GROUPS, client_socket, mutex);
        return;
    }

    char groups_names_list[LIST_OF_STRINGS_SIZE] = {0};
    give_all_groups_names(groups_manager, groups_names_list);
    send_message_with_1_string(groups_names_list, PRINT_EXISTING_GROUPS_SUCCESS, client_socket, mutex);
}

static void print_existing_users_request(GroupsManager* groups_manager, int client_socket, Mutex* mutex)
{
    if(num_of_groups(groups_manager) < 1)
    {
        send_only_message(PRINT_EXISTING_USERS_NO_USERS, client_socket, mutex);
        return;
    }

    char users_names_list[LIST_OF_STRINGS_SIZE] = {0};
    give_all_users_names(groups_manager, users_names_list);
    send_message_with_1_string(users_names_list, PRINT_EXISTING_USERS_SUCCESS, client_socket, mutex);
}

static void join_existing_request(GroupsManager* groups_manager, UsersManager* users_manager, char* buffer, int client_socket, Mutex* mutex)
{
    char user_name[STRING_SIZE]; char group_name[STRING_SIZE]; char return_ip[IP_SIZE]; 
    give_2_strings(buffer, user_name, group_name);

    GroupsManager_return g_result = join_existing_group(groups_manager, group_name, user_name, return_ip);
    switch (g_result)
    {
        case GROUPS_MANAGER_GROUP_NOT_EXISTS:
            send_message_with_1_string(group_name, JOIN_EXISTING_GROUP_GROUP_NOT_EXISTS, client_socket, mutex);
            return;

        case GROUPS_MANAGER_INSERT_CLIENT_FAIL:
            send_message_with_1_string(group_name, JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT, client_socket, mutex);
            return;

        case GROUPS_MANAGER_SUCCESS:
            break;
        
        default:
            printf("join_existing_group fail! GroupsManager_return: %d\n", g_result);
            send_only_message(JOIN_EXISTING_GROUP_FAIL, client_socket, mutex);
            return;
    }

    UsersManager_return u_result = user_join_group(users_manager, user_name, group_name);
    switch (u_result)
    {
        case USER_MANAGER_USER_NOT_EXISTS:
            leave_group(groups_manager, group_name, user_name);
            send_only_message(JOIN_EXISTING_GROUP_USER_NOT_EXISTS, client_socket, mutex);
            break;

        case USER_MANAGER_USER_ALREADY_IN_THE_GROUP:
            leave_group(groups_manager, group_name, user_name);
            send_message_with_1_string(group_name, JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT, client_socket, mutex);
            break;   

        case USER_MANAGER_SUCCESS:
            send_connect_to_group_success(group_name, return_ip, JOIN_EXISTING_GROUP_SUCCESS, client_socket, mutex);
            break;   
            
        default:
            printf("user_join_group fail! UsersManager_return: %d\n", u_result);
            leave_group(groups_manager, group_name, user_name);
            send_only_message(JOIN_EXISTING_GROUP_FAIL, client_socket, mutex);
            break;
    }
}

void get_buffer(ActionIn* action_in, char* buffer, int client_socket, Mutex* mutex)
{
    Message_type type = message_type(buffer);

    switch (type)
    {
        case REGISTRATION_REQUEST:
            registeration_request(action_in->m_subscribs_manager, buffer, client_socket, mutex);
            break;

        case LOG_IN_REQUEST:
            log_in_request(action_in->m_subscribs_manager, action_in->m_users_manager, buffer, client_socket, mutex);
            break;

        case EXIT_CHAT_REQUEST:
            leave_chat_request(action_in->m_users_manager, action_in->m_gruops_manager, buffer, client_socket, mutex);
            break; 

        case OPEN_NEW_GROUP_REQUEST:
            open_group_request(action_in->m_gruops_manager, action_in->m_users_manager, buffer, client_socket, mutex);
            break;

        case PRINT_EXISTING_GROUPS_REQUEST:
            print_existing_groups_request(action_in->m_gruops_manager, client_socket, mutex);
            break;

        case PRINT_EXISTING_USERS_REQUEST:
            print_existing_users_request(action_in->m_gruops_manager, client_socket, mutex);
            break;

        case JOIN_EXISTING_GROUP_REQUEST:
            join_existing_request(action_in->m_gruops_manager, action_in->m_users_manager, buffer, client_socket, mutex);
            break;

        case LEAVE_GROUP_REQUEST:
            leave_group_request(action_in->m_gruops_manager, action_in->m_users_manager, buffer, client_socket, mutex);
            break;

        case OPEN_PRIVATE_CHAT_REQUEST:
            open_group_request(action_in->m_gruops_manager, action_in->m_users_manager, buffer, client_socket, mutex);

        case PING_CLIENT_TO_SERVER:
            send_only_message(PING_CLIENT_TO_SERVER, client_socket, mutex);
            break;

        default:
            send_only_message(UNKNOWN_COMMAND, client_socket, mutex);
            break;
    }
}

void delete_disconnected_client(ActionIn* action_in, int client_socket)
{
    User* user = give_user_by_socket(action_in->m_users_manager, client_socket);
    if(user == NULL) {return;}

    List* group_list = user_groups_list(action_in->m_users_manager, user->m_name);
    if(group_list == NULL) {return;}

    //need to fix for private chats 
    GroupsManager_return resoult = leave_all_groups(action_in->m_gruops_manager, group_list, user->m_name);
    if(resoult != GROUPS_MANAGER_SUCCESS) {printf("leave_all_groups fail\n");}

    user_log_out(action_in->m_users_manager,user->m_name);
}


