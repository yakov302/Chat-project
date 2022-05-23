#include "server_manager.h"

static CreateInputs *CreateInputsStruct(AcceptClient _newClient, ReceiveMessage _getMessage, CloseClient _removeClient, Fail _failErr, void *_application);
static int NewClient(int _clientId, void *_application);
static void GetMessage(Server *_server, int _clientId, void *_message, int _messageSize, void *_application);
static void RemoveClient(int _clientId, void *_application);
static void FailErr(Server *_server, ServerErr _failErr, char *_perror, void *_application);
static int RegistrationRequest(void *_application, char *_buffer, void *_message, int _messageSize);
static int LogInReqest(void *_application, char *_buffer, void *_message, int _messageSize);
static int OpenNewGeoupReqest(void *_application, char *_buffer, void *_message, int _messageSize);
static int JoinExistingGroupReqest(void *_application, char *_buffer, void *_message, int _messageSize);
static int LeaveGroupReqest(void *_application, char *_buffer, void *_message, int _messageSize);
static int ExitChatReqest(void *_application, char *_buffer, void *_message, int _messageSize);

Application *CreateServerApplication()
{
	Application *application;
	application = (Application *)malloc(sizeof(application));
	if (application == NULL)
	{
		return NULL;
	}

	application->m_input = CreateInputsStruct(NULL, GetMessage, NULL, FailErr, application);
	if (application->m_input == NULL)
	{
		free(application);
		return NULL;
	}

	application->m_server = CreateServer(application->m_input);
	if (application->m_server == NULL)
	{
		free(application->m_input);
		free(application);
		return NULL;
	}

	application->m_users = CreateUserManager(application->m_input->m_maxSokets);
	if (application->m_users == NULL)
	{
		DestroyServer(application->m_server);
		free(application->m_input);
		free(application);
		return NULL;
	}

	application->m_groups = CreateGroupsManager(MAX_GROUPS);
	if (application->m_groups == NULL)
	{
		DestroyUserManager(application->m_users);
		DestroyServer(application->m_server);
		free(application->m_input);
		free(application);
		return NULL;
	}

	application->m_magicNumber = MAGIC_NUMBER;
	return application;
}

void DestroyServerApplication(Application *_application)
{
	if (_application == NULL || _application->m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}

	_application->m_magicNumber = 0;

	DestroyGroupsManager(_application->m_groups);
	DestroyUserManager(_application->m_users);
	DestroyServer(_application->m_server);
	// free(_application->m_input); bug - double free ***NEED TO CHECK***
	free(_application);
}

static void *ThreadFunction(void *_server)
{
	RunServer((Server *)_server);
	return NULL;
}

void RunApplication(Application *application)
{
	pthread_t m_tid;
	int res;
	m_tid = 0;

	res = pthread_create(&m_tid, 0, ThreadFunction, application->m_server);
	if (res != 0)
	{
		printf("server thread fail!\n");
	}
}

static CreateInputs *CreateInputsStruct(AcceptClient _newClient, ReceiveMessage _getMessage, CloseClient _removeClient, Fail _failErr, void *_application)
{
	CreateInputs *inputs;
	if (_getMessage == NULL || _failErr == NULL)
	{
		return NULL;
	}

	if ((inputs = (CreateInputs *)malloc(sizeof(CreateInputs))) == NULL)
	{
		return NULL;
	}

	inputs->m_acceptClient = _newClient;
	inputs->m_receiveMessage = _getMessage;
	inputs->m_closeClient = _removeClient;
	inputs->m_fail = _failErr;
	inputs->m_clientQueueSize = QUE_SIZE;
	inputs->m_maxSokets = MAX_SOCKETS;
	inputs->m_context = _application;
	inputs->m_maxMessageZize = BUFFER_SIZE;
	return inputs;
}

static int WhatToDoNow(char _type, char *_buffer, void *_message, int _messageSize, void *_application)
{
	int status, length;
	char strGroups[500] = {0};

	switch (_type)
	{
	case REGISTRATION_REQUEST:
		return RegistrationRequest(_application, _buffer, _message, _messageSize);

	case LOG_IN_REQUEST:
		return LogInReqest(_application, _buffer, _message, _messageSize);

	case OPEN_NEW_GROUP_REQUEST:
		return OpenNewGeoupReqest(_application, _buffer, _message, _messageSize);

	case PRINT_EXISTING_GROUPS_REQUEST:
		giveGroups(((Application *)_application)->m_groups, strGroups);
		length = PackStringMassage(strGroups, _buffer, PRINT_EXISTING_GROUPS_SUCCESS);
		return length;

	case JOIN_EXISTING_GROUP_REQUEST:
		return JoinExistingGroupReqest(_application, _buffer, _message, _messageSize);

	case LEAVE_GROUP_REQUEST:
		return LeaveGroupReqest(_application, _buffer, _message, _messageSize);

	case EXIT_CHAT_REQUEST:
		return ExitChatReqest(_application,_buffer, _message, _messageSize);
	}
}

static int RegistrationRequest(void *_application, char *_buffer, void *_message, int _messageSize)
{
	FirstAndSecond user;
	int status, length;

	UnpackFirstAndSecond(&user, _message, _messageSize);
	status = CreateUser(&user, ((Application *)_application)->m_users, NOT_FROME_LOWD);
	if (status == DUPLICATE_USERNAME)
	{
		length = PackStatusMassage(_buffer, REGISTRATION_REQUEST_DUPLICATE_USERNAME);
	}
	else if (status == SUCCESS)
	{
		length = PackStatusMassage(_buffer, REGISTRATION_REQUEST_SUCCESS);
	}
	else
	{
		length = PackStatusMassage(_buffer, REGISTRATION_REQUEST_FAIL);
	}
	return length;
}

static int LogInReqest(void *_application, char *_buffer, void *_message, int _messageSize)
{
	FirstAndSecond user;
	int status, length;

	UnpackFirstAndSecond(&user, _message, _messageSize);
	status = LogInUser(&user, ((Application *)_application)->m_users);
	if (status == WRONG_DETAILS)
	{
		length = PackStatusMassage(_buffer, LOG_IN_REQUEST_WRONG_DETAILS);
	}
	else if (status == SUCCESS)
	{
		length = PackStatusMassage(_buffer, LOG_IN_REQUEST_SUCCESS);
	}
	else if (status == ALREADY_LOG_IN)
	{
		length = PackStatusMassage(_buffer, ALREADY_LOG_IN);
	}
	else
	{
		length = PackStatusMassage(_buffer, LOG_IN_REQUEST_FAIL);
	}

	return length;
}

static int OpenNewGeoupReqest(void *_application, char *_buffer, void *_message, int _messageSize)
{
	FirstAndSecond user;
	int status, status2, length;
	static char name[30];
	char str[30];

	UnpackFirstAndSecond(&user, _message, _messageSize);
	strcpy(name, user.m_first);
	status = CreateGroup(user.m_first, ((Application *)_application)->m_groups, str);
	if (status == GROUP_SUCCESS)
	{
		status2 = UserJoinGroup(((Application *)_application)->m_users, user.m_second, user.m_first);
		if (status2 == SUCCESS)
		{
			strcpy(user.m_first, str);
			strcpy(user.m_second, name);
			length = PackFirstAndSecond(&user, _buffer, OPEN_NEW_GROUP_SUCCESS);
		}
		else
		{
			length = PackStatusMassage(_buffer, OPEN_NEW_GROUP_FAIL);
		}
	}
	else if (status == DUPLICATE_GROUP_NAME_FAIL)
	{
		length = PackStatusMassage(_buffer, DUPLICATE_GROUP_NAME);
	}
	else
	{
		length = PackStatusMassage(_buffer, OPEN_NEW_GROUP_FAIL);
	}
	return length;
}

static int JoinExistingGroupReqest(void *_application, char *_buffer, void *_message, int _messageSize)
{
	FirstAndSecond user;
	int status, status2, length;
	static char name[30];
	char str[30];

	UnpackFirstAndSecond(&user, _message, _messageSize);
	strcpy(name, user.m_first);
	status = JoinExistingGroup(user.m_first, ((Application *)_application)->m_groups, str);
	if (status == GROUP_SUCCESS)
	{
		status2 = UserJoinGroup(((Application *)_application)->m_users, user.m_second, user.m_first);
		if (status2 == CONNECT_TO_SAME_GROUP)
		{
			length = PackStatusMassage(_buffer, DUPLICATE_GROUP_CONNECT);
		}
		else if (status2 == SUCCESS)
		{
			strcpy(user.m_first, str);
			strcpy(user.m_second, name);
			length = PackFirstAndSecond(&user, _buffer, JOIN_EXISTING_GROUP_SUCCESS);
		}
		else
		{
			length = PackStatusMassage(_buffer, JOIN_EXISTING_GROUP_FAIL);
		}
	}
	else if (status == GROUP_NOT_FOUND_IN_HASH)
	{
		length = PackStatusMassage(_buffer, GROUP_NOT_FOUND);
	}
	else
	{
		length = PackStatusMassage(_buffer, JOIN_EXISTING_GROUP_FAIL);
	}
	return length;
}

static int LeaveGroupReqest(void *_application, char *_buffer, void *_message, int _messageSize)
{
	FirstAndSecond user;
	int status, status2, length;
	static char name[30];
	char str[30];

	UnpackFirstAndSecond(&user, _message, _messageSize);
	strcpy(name, user.m_first);
	status = LeaveGroup(user.m_first, ((Application *)_application)->m_groups, str);
	if (status == GROUP_SUCCESS)
	{
		status2 = UserLeaveGroup(((Application *)_application)->m_users, user.m_second, user.m_first);
		if (status2 == SUCCESS)
		{
			strcpy(user.m_second, user.m_first);
			length = PackFirstAndSecond(&user, _buffer, LEAVE_GROUP_SUCCESS);
		}
		else if (status2 == NO_FOUND_IN_HASH)
		{
			length = PackStatusMassage(_buffer, GROUP_NOT_FOUND);
		}
		else
		{
			length = PackStatusMassage(_buffer, LEAVE_GROUP_FAIL);
		}
	}
	else if (status == GROUP_NOT_FOUND_IN_HASH)
	{
		length = PackStatusMassage(_buffer, GROUP_NOT_FOUND);
	}
	else if (status == GROUP_DELETE)
	{
		status2 = UserLeaveGroup(((Application *)_application)->m_users, user.m_second, user.m_first);
		if (status2 == SUCCESS)
		{
			strcpy(user.m_second, user.m_first);
			length = PackFirstAndSecond(&user, _buffer, GROUP_DELETED);
		}
		else if (status2 == NO_FOUND_IN_HASH)
		{
			length = PackStatusMassage(_buffer, GROUP_NOT_FOUND);
		}
		else
		{
			length = PackStatusMassage(_buffer, LEAVE_GROUP_FAIL);
		}
	}
	else
	{
		length = PackStatusMassage(_buffer, LEAVE_GROUP_FAIL);
	}
	return length;
}

static int ExitChatReqest(void *_application, char *_buffer, void *_message, int _messageSize)
{
	int length;
	static char name[30];
	char str[30];
	
	UnpackStringMassage(str, _message, _messageSize);
	strcpy(name, str);
	UserLogOut(((Application *)_application)->m_users, name);
	length = PackStatusMassage(_buffer, LEAVE_CHAT_SUCCESS);

	return length;
}

static int NewClient(int _clientId, void *_application)
{
	return ACCEPT_CLIENT;
}

static void GetMessage(Server *_server, int _clientId, void *_message, int _messageSize, void *_application)
{
	int len;
	char type;
	char buffer[BUFFER_SIZE];
	type = ReturnMessageType(_message);
	len = WhatToDoNow(type, buffer, _message, _messageSize, _application);
	if (len != -1)
	{
		SendMessageToClien(_server, _clientId, buffer, len);
	}
}

static void RemoveClient(int _clientId, void *_application)
{

}

static void FailErr(Server *_server, ServerErr _failErr, char *_perror, void *_application)
{
	switch (_failErr)
	{
		case LIST_CREATE_FAIL:
			printf("List create fail: %s\n", _perror);
			break;

		case SOCKET_FAIL:
			printf("Socket fail: %s\n", _perror);
			break;

		case SETSOCKOPT_FAIL:
			printf("Setsockopt fail: %s\n", _perror);
			break;

		case BIND_FAIL:
			printf("Bind fail: %s\n", _perror);
			break;

		case LISTEN_FAIL:
			printf("Listen fail: %s\n", _perror);
			break;

		case SELECT_FAIL:
			printf("Select fail: %s\n", _perror);
			break;

		case DATA_MALLOC_FAIL:
			printf("Data malloc fail: %s\n", _perror);
			break;

		case LIST_PUSH_HEAD_FAIL:
			printf("List push head fail: %s\n", _perror);
			break;

		case ACCEPT_FAIL:
			printf("Accept fail: %s\n", _perror);
			break;

		case SERVER_FULL:
			printf("Server full: %s\n", _perror);
			break;

		case CONNECTION_CLOSED:
			printf("Connection closed: %s\n", _perror);
			break;

		case BUFFER_MALLOC_FAIL:
			printf("Buffer malloc fail: %s\n", _perror);
			break;

		case RECEIVE_FAIL:
			printf("Receive fail: %s\n", _perror);
			break;

		case SEND_FAIL:
			printf("Send fail: %s\n", _perror);
			break;
	}

	if (_failErr <= 8)
	{
		PauseServer(_server);
		DestroyServer(_server);
	}
}
