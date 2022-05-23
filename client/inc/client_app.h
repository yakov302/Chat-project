#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__

#include <unistd.h>
#include <signal.h>

#include "client_net.h"
#include "protocol.h"
#include "vector.h"
#include "ui.h"

#define MAGIC_NUMBER 65465198
#define BUFFER_SIZE 800
#define REGISTRATION 1
#define CREATE_GROUP 1
#define PRINT_GROUP 2
#define LEAVE_GROUP 4
#define JOIN_GROUP 3
#define NAME_SIZE 40
#define EXIT_CHAT 5
#define _GNU_SOURCE
#define LOG_IN 2
#define EXIT 3
#define OFF 0
#define ON 1

typedef struct GroupAndId 
{
	char m_name [NAME_SIZE];
	int m_idUser;
	int m_idGroup;
}GroupAndId;

typedef struct Client
{
	char m_name [NAME_SIZE];
	char m_passWord [NAME_SIZE];
	int m_socket;
	Vector* m_groupVector;
	int m_magicNumber;
}Client;

/*this funcion receive the user choice in the main menu of the program.
params - the user choice.
the function return Client*/
void RunMainMenu(int _resFromMenu);


#endif //__CLIENT_APP_H__



