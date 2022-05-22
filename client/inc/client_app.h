#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__

#include <unistd.h>
#include <signal.h>

#include "client_net.h"
#include "protocol.h"
#include "vector.h"
#include "ui.h"

#define NAME_SIZE 40

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
the function return void*/
void RunMainMenu(int _resFromMenu);


#endif 



