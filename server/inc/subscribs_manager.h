#ifndef SUBSCRIBS_MANAGER_H
#define SUBSCRIBS_MANAGER_H

#include <string.h>

#include "hash_map.h"
#include "sha256.h"

#define MAGIC_NUMBER 144522
#define STRING_SIZE 68
#define NOT_EQUAL 0
#define EQUAL 1

typedef struct Subscriber
{
	char m_name[STRING_SIZE];
	char m_password[STRING_SIZE];

}Subscriber;

typedef struct SubscribsManager
{
	HashMap* m_subscribs;
	int m_magic_number;

}SubscribsManager;

typedef enum SubscribsManager_return
{
	SUBSCRIBS_MANAGER_SUCCESS,
	OPEN_FILE_FAIL,
	SUBSCRIBS_MANAGER_USERNAME_ALREADY_EXISTS,
    SUBSCRIBS_MANAGER_USERNAME_NOT_EXISTS,
	SUBSCRIBS_MANAGER_MALLOC_FAIL,
    SUBSCRIBS_MANAGER_PUSH_FAIL,
    SUBSCRIBS_MANAGER_CREATE_FROM_FILE_FAIL,
    SUBSCRIBS_MANAGER_CREATE_SUBSCRIBER_FAIL,
    SUBSCRIBS_MANAGER_ARGS_NOT_INITIALIZED,
    SUBSCRIBS_MANAGER_WRITE_TO_FILE_FAIL,
    SUBSCRIBS_MANAGER_INVALID_PASSWORD

}SubscribsManager_return;

SubscribsManager* create_subscribs_manager(int capacity);

void destroy_subscribs_manager(SubscribsManager* subscribs_manager);

SubscribsManager_return new_subscriber(SubscribsManager* subscribs_manager, const char* name, char* password);

SubscribsManager_return log_in_chack(SubscribsManager* subscribs_manager, const char* name, char* password);


#endif // SUBSCRIBS_MANAGER_H
