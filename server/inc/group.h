#ifndef GROUP_H
#define GROUP_H

#include <stdlib.h>
#include <string.h>

#define MAGIC_NUMBER 144522
#define STRING_SIZE 68

typedef struct Group
{
	char m_name[STRING_SIZE];
	char m_ip[STRING_SIZE];
	int m_num_of_clients;
	int m_magic_number;

}Group;

Group* create_group(const char* group_name, char* group_ip);

void destroy_group(Group* group);

void group_ip(Group* group, char* group_ip);

int number_of_clients(Group* group);

void decrese_num_of_cleints (Group* group);

void increse_num_of_cleints (Group* group);


#endif //GROUP_H

