#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>

#include "protocol.h"

#define YELLOW "\x1B[33m"
#define NORMAL "\x1B[0m"
#define RED "\x1B[91m"
#define GREEN "\x1B[32m"

#define TRUE 1 
#define FALSE 0

int menu(void);

int is_logged_in();

void enter_user_name(char* user_name);

void enter_group_name(char* group_name);

void enter_password(char* password);

void print_invalid_choice();

void print_message(Message_type message_type);

void print_groups_names_list(char* groups_names_list);


#endif // IO_H