#ifndef GROUP_H
#define GROUP_H

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define COMMAND_SIZE 100
#define STRING_SIZE 68
#define ERROR -1
#define FALSE 0
#define TRUE 1

typedef struct Group
{
    char m_name[STRING_SIZE];
    int m_chat_window_process_id;
    int m_text_bar_process_id;

}Group;

Group* group_create(char* ip, int port, char* group_name, char* user_name);

void group_destroy(Group* group);


#endif // GROUP_H
