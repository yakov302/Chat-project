#include "client_app.h"

#define _GNU_SOURCE
#define BUFFER_SIZE 800
#define REGISTRATION 1
#define LOG_IN 2
#define EXIT 3
#define CREATE_GROUP 1
#define PRINT_GROUP 2
#define JOIN_GROUP 3
#define LEAVE_GROUP 4
#define EXIT_CHAT 5
#define MAGIC_NUMBER 65465198

int flag = 0;

static char SendToProtocol(char* _buffer);
static int registrationToPack(Client* _client ,char* _buffer, MessagesTypes _type);
static void WhatToDoClient(Client* _client, char _messageType, char* _buffer);
static ClientResult ClientRunApp(Client* _client, char _choice);
static void ClientDestroyApp(Client* _client);
static ClientResult ClientRunAppSecondMenu(Client* _client, int _choice);
static void RunSecondMenu(Client* _client, int _resFromMenu);
static int NewGroupToPack(Client* _client, char* _buffer, MessagesTypes _type);
static int NewGroupToPackAuto(Client* _client, char* _buffer, MessagesTypes _type, char* _name);
static int NameToPack(Client* _client, char* _buffer, MessagesTypes _type);
static int CloseId(void* _element, size_t _index, void* _context);
static int GroupWindow(char* _ip, char* _groupName);
static GroupAndId* groupAndIdCreate(void);
static void groupAndIdDestroy(void* _newgroupAndId);
static int UserWindow(char* _ip,char* _userName, char* _groupName);
static MessagesTypes GiveMessagesType(char _choice);
static MessagesTypes GiveMessagesTypeSecondMenu(char _choice);

/**************************ClientCreate***************************/

static Client* ClientCreate(void)
{
	Client* newClient;
	struct sockaddr_in sin;

	newClient = (Client*) malloc (sizeof(Client));
	if (newClient == NULL)
	{
       	return NULL;
   	}

   	newClient->m_groupVector = VectorCreate(20, 5);
   	newClient -> m_magicNumber = MAGIC_NUMBER;
   	newClient -> m_socket = ClientInitialization(&sin);

	return newClient;
}

/*******************DestroyClient***********************/

static void DestroyClient(Client* _client)
{
	if(_client == NULL || _client->m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	_client -> m_magicNumber = 0;
	close(_client -> m_socket);
	free(_client);
}

/******************RunMainMenu*********************/

void RunMainMenu(int _resFromMenu)
{
	Client* newClient = NULL;
	int resFromMenu = _resFromMenu;

	while (flag == 0)
	{
		switch(resFromMenu)
      	{
			case REGISTRATION:
				newClient = ClientCreate();
				ClientRunApp(newClient, resFromMenu);
				resFromMenu = MainMenu();
         		RunMainMenu(resFromMenu);
				break;
					
			case LOG_IN:
				if(newClient == NULL)
				{
					newClient = ClientCreate();
				}
				ClientRunApp(newClient, resFromMenu);
				flag = 1;
				break;
					
			case EXIT:
				PrintToClient(LEAVE_CHAT_SUCCESS);
				flag = 1;
				break;
						
			default:
				PrintInvalidChoice();
				resFromMenu = MainMenu();   
				break;
		}
	}
}	

/********************ClientRunApp*******************/

static ClientResult ClientRunApp(Client* _client, char _choice)
{
	char* buffer;
	MessagesTypes msgType;

	buffer = (char*) malloc (BUFFER_SIZE * sizeof(char));
	if (buffer == NULL)
	{
		return CLIENT_ALLOCATION_FAIL;
	}
	
	if(_client == NULL)
	{
		return CLIENT_NULL;
	}
	
	msgType = GiveMessagesType(_choice);	
	WhatToDoClient(_client, msgType, buffer);
	msgType = ReturnMessageType(buffer);
	WhatToDoClient(_client, msgType, buffer);
	free(buffer);
}

static MessagesTypes GiveMessagesType(char _choice)
{
	if (_choice == REGISTRATION)
	{
		return REGISTRATION_REQUEST;
	}
	if (_choice == LOG_IN)
	{
		return  LOG_IN_REQUEST;
	}
	if (_choice == EXIT)
	{
		return LEAVE_CHAT_REQUEST;
	}
	return LEAVE_CHAT_REQUEST;
}

/**********************WhatToDoClient***********************/

static void WhatToDoClient(Client* _client, char _messageType, char* _buffer)
{
	int len, resFromMenu, groupId, userId;
	char name[IP_SIZE]; 
	char ip[IP_SIZE];
	char str[BUFFER_SIZE];
	char groups[800];
	size_t index, endOfVector;
	FirstAndSecond ipAndPort;
	GroupAndId* newgroupAndId;
	void* item1;
	void* item2;
	void*pValue;
	size_t size; 
	MessagesTypes msgType;

	switch(_messageType)
	{
		case REGISTRATION_REQUEST:
			len = registrationToPack(_client, _buffer, REGISTRATION_REQUEST);
			RunClientNet(_client, _buffer, len);               
       		break;
       			
        case REGISTRATION_REQUEST_SUCCESS:
         	PrintToClient(REGISTRATION_REQUEST_SUCCESS);
         	break;
         		
        case REGISTRATION_REQUEST_DUPLICATE_USERNAME:
        	PrintToClient(REGISTRATION_REQUEST_DUPLICATE_USERNAME);
        	break;
        		 
    	case REGISTRATION_REQUEST_FAIL:
        	PrintToClient(REGISTRATION_REQUEST_FAIL);
        	break;
        	  
        case LOG_IN_REQUEST:
         	len = registrationToPack(_client, _buffer, LOG_IN_REQUEST);
        	RunClientNet(_client, _buffer, len);
        	break;
        		 
    	case LOG_IN_REQUEST_SUCCESS:
          	PrintToClient(LOG_IN_REQUEST_SUCCESS);
			resFromMenu = SecondMenu();
			RunSecondMenu(_client, resFromMenu);
        	break;
        		 
        case LOG_IN_REQUEST_FAIL:
           	PrintToClient(LOG_IN_REQUEST_FAIL);
         	resFromMenu = MainMenu();
            RunMainMenu(resFromMenu);
         	break;
        		 
        case LOG_IN_REQUEST_WRONG_DETAILS:
         	PrintToClient(LOG_IN_REQUEST_WRONG_DETAILS);
        	resFromMenu = MainMenu();
            RunMainMenu(resFromMenu);
         	break;
		
		case ALREADY_LOG_IN:
			PrintToClient(ALREADY_LOG_IN);
        	resFromMenu = MainMenu();
            RunMainMenu(resFromMenu);
         	break;
      		       		
        case LEAVE_CHAT_REQUEST:
 			len = NameToPack(_client, _buffer, LEAVE_CHAT_REQUEST);
	       	SendRecive(_client, _buffer, len);
         	break;
         		
	    case LEAVE_CHAT_SUCCESS:
           	PrintToClient(LEAVE_CHAT_SUCCESS);
           	ClientDestroyApp(_client);
         	break;

	    case OPEN_NEW_GROUP_REQUEST:
	       	len = NewGroupToPack(_client, _buffer, OPEN_NEW_GROUP_REQUEST);
	       	SendRecive(_client, _buffer, len);
	     	break;
	   	  		
	    case OPEN_NEW_GROUP_SUCCESS:
	       	len = ReturnMessageSize(_buffer);
	       	UnpackFirstAndSecond(&ipAndPort, _buffer, len);
	       	PrintToClient(OPEN_NEW_GROUP_SUCCESS);
	       	newgroupAndId = groupAndIdCreate();
	       	groupId = GroupWindow(ipAndPort.m_first, ipAndPort.m_second);
	       	userId = UserWindow(ipAndPort.m_first, _client ->m_name,  ipAndPort.m_second);
	       	strcpy(newgroupAndId-> m_name, ipAndPort.m_second);
	       	newgroupAndId->m_idGroup = groupId;
	       	newgroupAndId->m_idUser = userId;
	       	VectorAppend(_client->m_groupVector, newgroupAndId);
	     	break;
	     		
	    case OPEN_NEW_GROUP_FAIL:
	      	PrintToClient(OPEN_NEW_GROUP_FAIL);
	     	break;
	     		
	    case DUPLICATE_GROUP_NAME:
	       	PrintToClient(DUPLICATE_GROUP_NAME);
	       	resFromMenu = SecondMenu();
	       	RunSecondMenu(_client, resFromMenu);
	     	break;
	     		
	    case PRINT_EXISTING_GROUPS_REQUEST:
	       	len = PackStatusMassage(_buffer, PRINT_EXISTING_GROUPS_REQUEST);
	       	SendRecive(_client, _buffer, len);
	       	break;
	     		
	    case JOIN_EXISTING_GROUP_REQUEST:      		
	       	len = NewGroupToPack(_client, _buffer, JOIN_EXISTING_GROUP_REQUEST);
	       	SendRecive(_client, _buffer, len); 	        		
	     	break;
	     		
	    case PRINT_EXISTING_GROUPS_SUCCESS:
	       	len = ReturnMessageSize( _buffer);
	       	UnpackStringMassage(groups,  _buffer, len);
	       	printGroups(groups);
	       	break;
	       		
	    case NO_EXISTING_GROUPS:
	       	len = ReturnMessageSize(_buffer);
	       	UnpackStringMassage(str, _buffer, len);
	       	printGroupsNames(str);
	   		break;
	   		  
	    case JOIN_EXISTING_GROUP_SUCCESS:
	       	len = ReturnMessageSize(_buffer);
	       	UnpackFirstAndSecond(&ipAndPort, _buffer, len);
	       	PrintToClient(JOIN_EXISTING_GROUP_SUCCESS);
	       	newgroupAndId = groupAndIdCreate();
	       	groupId = GroupWindow(ipAndPort.m_first, ipAndPort.m_second);
	       	userId = UserWindow(ipAndPort.m_first, _client ->m_name,  ipAndPort.m_second);
	       	strcpy(newgroupAndId-> m_name, ipAndPort.m_second);
	       	newgroupAndId->m_idGroup = groupId;
	       	newgroupAndId->m_idUser = userId;
	       	VectorAppend(_client->m_groupVector, newgroupAndId);
	     	break;
	     
	    case JOIN_EXISTING_GROUP_FAIL:
	       	PrintToClient(JOIN_EXISTING_GROUP_FAIL);
			break;

		case DUPLICATE_GROUP_CONNECT:
	       	PrintToClient(DUPLICATE_GROUP_CONNECT);;
			break;

	    case GROUP_NOT_FOUND:
	       	PrintToClient(GROUP_NOT_FOUND);
	     	break;
	     
	    case LEAVE_GROUP_REQUEST:
	     	len =  NewGroupToPack(_client, _buffer, LEAVE_GROUP_REQUEST);
	       	SendRecive(_client, _buffer, len); 
	     	break;
	     
	    case LEAVE_GROUP_SUCCESS:
	       	len = ReturnMessageSize(_buffer);
	       	UnpackFirstAndSecond(&ipAndPort, _buffer, len);
	       	PrintToClient(LEAVE_GROUP_SUCCESS);
	       	index = VectorForEach(_client->m_groupVector, CloseId, ipAndPort.m_second);
	       	endOfVector = VectorSize(_client->m_groupVector);
	       	VectorGet(_client->m_groupVector, index, &item1);
	       	VectorGet(_client->m_groupVector, endOfVector-1, &item2);
	       	VectorSet(_client->m_groupVector, index, item2);
	       	VectorSet(_client->m_groupVector, endOfVector-1, item1);
	       	VectorRemove(_client->m_groupVector, &pValue);
	     	break;
	     		
	    case LEAVE_GROUP_FAIL:
	      	len = ReturnMessageSize(_buffer);
	       	UnpackFirstAndSecond(&ipAndPort, _buffer, len);
	      	PrintToClient(LEAVE_GROUP_FAIL);
	     	break;
	     	     	
	    case GROUP_DELETED:
	       	len = ReturnMessageSize(_buffer);
	       	UnpackFirstAndSecond(&ipAndPort, _buffer, len);
	  		PrintToClient(GROUP_DELETED);
	  		index = VectorForEach(_client->m_groupVector, CloseId, ipAndPort.m_second);
	       	endOfVector = VectorSize(_client->m_groupVector);
	       	VectorGet(_client->m_groupVector, index, &item1);
	       	VectorGet(_client->m_groupVector, endOfVector-1, &item2);
	       	VectorSet(_client->m_groupVector, index, item2);
	       	VectorSet(_client->m_groupVector, endOfVector-1, item1);
	       	VectorRemove(_client->m_groupVector, &pValue);
	     	break;
		
		case EXIT_CHAT_REQUEST:
		 	size = VectorSize (_client->m_groupVector);
			for(int i = 0; i < size; ++i)
			{
				VectorGet(_client->m_groupVector, 0, (void*)&newgroupAndId);
				strcpy(name, newgroupAndId->m_name);
				printf("\n\n name client : %s \n\n", name);
	     		len =  NewGroupToPackAuto(_client, _buffer, LEAVE_GROUP_REQUEST, name);
	       		SendRecive(_client, _buffer, len);
				msgType = ReturnMessageType(_buffer);
				WhatToDoClient(_client, msgType, _buffer); 
			}
			len = NameToPack(_client, _buffer, EXIT_CHAT_REQUEST);
	       	SendRecive(_client, _buffer, len);
         	break;
	     
        default:
         	PrintInvalidChoice();
         	break;
	}
}

/********************GroupWindow************************/

static int GroupWindow(char* _ip, char* _groupName)
{
	int pidUser;
	FILE* fgroup;
   
    char command[100];
    sprintf(command, "gnome-terminal -q --geometry=55x13 --title=%s -- ./groupchat.out %s %s",_groupName, _ip, _groupName);            
    system(command);
    usleep(10000);
	fgroup = fopen("ip.txt","r");
	fscanf(fgroup,"%d", &pidUser);
    fclose(fgroup);

    return pidUser;
}

/********************UserWindow************************/

static int UserWindow(char* _ip,char* _userName, char* _groupName)
{
	int pidGroup;
	FILE *fuser;
	
	char command[100];
    sprintf(command, "gnome-terminal -q --geometry=55x3 --title=%s -- ./userchat.out %s %s %s",_groupName, _ip, _userName, _groupName);
    system(command);
    usleep(10000);
	fuser = fopen("ip2.txt","r");
    fscanf(fuser,"%d",&pidGroup);
    fclose(fuser);

    return pidGroup;
}

/********************CloseId************************/

static int CloseId(void* _element, size_t _index, void* _context)
{
	int res;
	int pidUser, pidGroup;

	 if (_context == NULL)
	 {
		kill( ((GroupAndId*)_element) -> m_idUser , SIGKILL);
		kill( ((GroupAndId*)_element) -> m_idGroup , SIGKILL);
		return 1;
	 }

	 res = strcmp(((GroupAndId*)_element) -> m_name , (char*)_context);
	 if (res == 0)
	 {
		kill( ((GroupAndId*)_element) -> m_idUser , SIGKILL);
		kill( ((GroupAndId*)_element) -> m_idGroup , SIGKILL);
		return 0;
	 }
	return 1;
}

/********************registrationToPack************************/

static int registrationToPack(Client* _client, char* _buffer, MessagesTypes _type)
{
	FirstAndSecond* nap = (FirstAndSecond*) malloc (sizeof( FirstAndSecond));
	char name[NAME_SIZE];
	char password[NAME_SIZE];
	int res;

	InsertName(name);
	InsertPassWord(password);
	strcpy(_client -> m_name, name);
	strcpy(_client -> m_passWord, password);
	strcpy(nap -> m_first, name);
	strcpy(nap -> m_second, password);
	
	res = PackFirstAndSecond(nap, _buffer, _type);
	free(nap);
	return res;
}

/********************NewGroupToPack************************/

static int NewGroupToPack(Client* _client, char* _buffer, MessagesTypes _type)
{
	FirstAndSecond* newGroup = (FirstAndSecond*) malloc (sizeof( FirstAndSecond));
	char groupName[NAME_SIZE];
	int res;

	InsertGroupName(groupName);
	strcpy( newGroup -> m_first, groupName);
	strcpy(newGroup -> m_second, _client -> m_name);
	
	res = PackFirstAndSecond(newGroup, _buffer, _type);
	free(newGroup);
	return res;
}

static int NewGroupToPackAuto(Client* _client, char* _buffer, MessagesTypes _type, char* _name)
{
	FirstAndSecond* newGroup = (FirstAndSecond*) malloc (sizeof( FirstAndSecond));
	int res;

	strcpy( newGroup -> m_first, _name);
	strcpy(newGroup -> m_second, _client -> m_name);
	
	res = PackFirstAndSecond(newGroup, _buffer, _type);
	free(newGroup);
	return res;
}

/********************NameToPack************************/

static int NameToPack(Client* _client, char* _buffer, MessagesTypes _type)
{
	char clientName[NAME_SIZE];
	strcpy(clientName, _client->m_name);
	return PackStringMassage(clientName ,_buffer, _type);
}

/********************ClientRunAppSecondMenu************************/

static ClientResult ClientRunAppSecondMenu(Client* _client, int _choice)
{
	char* buffer;
	MessagesTypes msgType;

	buffer = (char*) malloc (BUFFER_SIZE * sizeof(char));
	if (buffer == NULL)
	{
		return CLIENT_ALLOCATION_FAIL;
	}
	
	if(_client == NULL)
	{
		return CLIENT_NULL;
	}

	msgType = GiveMessagesTypeSecondMenu(_choice);
	WhatToDoClient(_client, msgType, buffer);
	msgType = ReturnMessageType(buffer);
	WhatToDoClient(_client, msgType, buffer);
	free(buffer);
}

static MessagesTypes GiveMessagesTypeSecondMenu(char _choice)
{
	if (_choice == CREATE_GROUP)
	{
		return OPEN_NEW_GROUP_REQUEST;
	}
	
	if (_choice == PRINT_GROUP)
	{
		return PRINT_EXISTING_GROUPS_REQUEST;
	}
	
	if (_choice == JOIN_GROUP)
	{
		return JOIN_EXISTING_GROUP_REQUEST;
	}
	
	if (_choice == LEAVE_GROUP)
	{
		return LEAVE_GROUP_REQUEST;
	}
	
	if (_choice == EXIT_CHAT)
	{
		return EXIT_CHAT_REQUEST;
	}
	return EXIT_CHAT_REQUEST;
}

/*********************groupAndIdDestroy*********************/

static void groupAndIdDestroy(void* _newgroupAndId)
{
	if(_newgroupAndId == NULL)
	{
		return;
	}
	free(_newgroupAndId);
}

/*********************ClientDestroyApp*********************/

static void ClientDestroyApp(Client* _client)
{
	if(_client == NULL)
	{
		return;
	}

	VectorDestroy (&_client -> m_groupVector , groupAndIdDestroy);
	DestroyClient(_client);
}

/********************SendToProtocol***********************/

static char SendToProtocol(char* _buffer)
{
	char messageType;
	if (_buffer == NULL)
	{
       	return CLIENT_FAIL;
   	}
   	messageType = ReturnMessageType(_buffer);
	return messageType;
}

/******************RunSecondMenu*******************/

static void RunSecondMenu(Client* _client, int _resFromMenu)
{
	int resFromMenu = _resFromMenu;

	while (flag == 0)
	{
		switch(resFromMenu)
		{
			case CREATE_GROUP:
           		ClientRunAppSecondMenu(_client, CREATE_GROUP);
				resFromMenu = SecondMenu();
	       		RunSecondMenu(_client, resFromMenu);
           		break;

           	case PRINT_GROUP:
           		ClientRunAppSecondMenu(_client, PRINT_GROUP);
				resFromMenu = SecondMenu();
	       		RunSecondMenu(_client, resFromMenu);
           		break;

			case JOIN_GROUP:
				ClientRunAppSecondMenu(_client, JOIN_GROUP);
				resFromMenu = SecondMenu();
	       		RunSecondMenu(_client, resFromMenu);
           		break;

          	case LEAVE_GROUP:
           		ClientRunAppSecondMenu(_client, LEAVE_GROUP);
				resFromMenu = SecondMenu();
	       		RunSecondMenu(_client, resFromMenu);
            	break;
            			
            case EXIT_CHAT:
            	flag = 1;
            	ClientRunAppSecondMenu(_client, EXIT_CHAT);
            	break;
  
        	default:
           		PrintInvalidChoice();
				resFromMenu = SecondMenu();
				RunSecondMenu(_client, resFromMenu);
           		break;         			
		}
   	}	
}

/**************************groupAndIdCreate***************************/

static GroupAndId* groupAndIdCreate(void)
{
	GroupAndId* newgroupAndId;
	newgroupAndId = (GroupAndId*) calloc (1, sizeof(GroupAndId));
	if (newgroupAndId == NULL)
	{
       	return NULL;
   	}

   	return newgroupAndId;
}

