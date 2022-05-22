#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include <pthread.h>

#include "server_net.h"
#include "protocol.h"
#include "groups_manager.h"
#include "users_manager.h"

typedef struct Application
{
	Server* m_server;
	CreateInputs* m_input;
	UserMng* m_users;
	GrupsMng* m_groups;
	int m_magicNumber;
}Application;

/** This function create the server application with all the data structures and databases.
	There is no input.
	Output: pointer to struct Application.
	return NULL if one of the allocations fails**/
Application* CreateServerApplication ();

/** This function run the server-net and prepare the application to get and recive data.
Input: pointer to application created above.
Output: void.
Errors: No potential errors. **/
void RunApplication (Application* application);

/**This function didn't used in the application. It's destroy the application and free all allocations.
Input: pointer to application created above.
Output: void.**/
void DestroyServerApplication (Application* _application);


#endif
