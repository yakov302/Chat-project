#ifndef IO_H
#define IO_H

#include "protocol.h"

#include <poll.h>
#include <unistd.h>

#define YELLOW "\x1B[33m"
#define NORMAL "\x1B[0m"
#define GREEN "\x1B[32m"
#define RED "\x1B[91m"

#define TRUE 1 
#define FALSE 0
#define ERROR -1
#define COMMAND_SIZE 256
#define MESSAGE_SIZE 100
#define GET_OUT_FROM_SCANF 187


int menu(int is_looged_in);

int is_logged_in();

void enter_user_name(char* user_name);

void enter_group_name(char* group_name);

void enter_password(char* password);

void print_invalid_choice();

void print_exit_chat(char* name);

void print_message(Message_type message_type, char* string);

void print_buffer(char* names_list);


#endif // IO_H