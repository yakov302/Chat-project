#include "group.h"

static int read_chat_window_process_id_from_file()
{
    FILE* file;
    int process_id;

	file = fopen("../resources/chat_window_process_id.txt","r");
    if(file == NULL)
	{
		perror("fopen fail!\n");
		return ERROR;
	}

	fscanf(file,"%d", &process_id);
    fclose(file);
    return process_id;
}

static int read_text_bar_process_id_from_file()
{
    FILE* file;
    int process_id;

	file = fopen("../resources/text_bar_process_id.txt","r");
    if(file == NULL)
	{
		perror("fopen fail!\n");
		return ERROR;
	}

	fscanf(file,"%d", &process_id);
    fclose(file);
    return process_id;
}

static int open_chat_window(char* ip, int port, char* group_name)
{   
    char command[COMMAND_SIZE];
    sprintf(command, "gnome-terminal -q --geometry=55x13 --title=%s -- ./chat_window %s %d", group_name, ip, port);            
    int result = system(command);
    if(result == ERROR) {perror("system fail!"); return ERROR;}
    return read_chat_window_process_id_from_file();
}

static int open_text_bar_window(char* ip, int port, char* group_name, char* user_name)
{
    char command[COMMAND_SIZE];
    sprintf(command, "gnome-terminal -q --geometry=55x3 --title=%s -- ./text_bar %s %d %s",group_name, ip, port, user_name);            
    int result = system(command);
    if(result == ERROR) {perror("system fail!"); return ERROR;}
    return read_text_bar_process_id_from_file();
}

Group* group_create(char* ip, int port, char* group_name, char* user_name)
{
	if(ip == NULL || group_name == NULL || user_name == NULL)
	{printf("uninitialized arguments!\n"); return NULL;}

	Group* group = (Group*)malloc(sizeof(Group));
	if(group == NULL) {printf("malloc fail!\n"); return NULL;}

	strcpy(group->m_name, group_name);
	group->m_chat_window_process_id = open_chat_window(ip, port, group_name);
	group->m_text_bar_process_id = open_text_bar_window(ip, port, group_name, user_name);
	if(group->m_chat_window_process_id == ERROR || group->m_text_bar_process_id == ERROR)
	{printf("open windows fail!\n"); return NULL;}

	return group;
}

void group_destroy(Group* group)
{
	if(group == NULL)
		return;
	
	kill(group->m_chat_window_process_id , SIGKILL);
	kill(group->m_text_bar_process_id , SIGKILL);
	free(group);
	group = NULL;
}

