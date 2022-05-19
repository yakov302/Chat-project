#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "groups_manager.h"
#include "users_manager.h"
#include "server_net.h"
#include "gen.list.h"
#include "protocol.h"
#include "list.h"

typedef struct Application Application;

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
