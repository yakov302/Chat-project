# include "../inc/subscribs_manager.h"

static size_t hash_for_subscriber_name(void* name)
{
	size_t mul = 1;    
	const int len = strlen ((char*)name);

	for (int i = 0; i < len; ++i)
		mul *= ((char*)name)[i]+((i+1));

	return mul;
}

static int compare_subscriber_names (const void* hash_element, const void* subscriber_name)	
{
	if (strcmp(((Subscriber*)((Element*)hash_element)->m_value)->m_name, (char*)subscriber_name) == 0)
		return EQUAL;

	return NOT_EQUAL;
}

static void destroy_subscriber_element(void* element) 
{
    if(element != NULL)
    {
	    free(((Element*)element)->m_value);
        free(((Element*)element)->m_key);
        free(element);
    }
}

static Subscriber* create_subscriber_from_file(FILE* file, SubscribsManager* subscribs_nanager)
{
	Subscriber* subscriber = (Subscriber*)malloc(sizeof(Subscriber));
    if(subscriber == NULL) {return NULL;}

	fgets(subscriber->m_name, STRING_SIZE, file);
    fgets(subscriber->m_password, STRING_SIZE, file);

    subscriber->m_name[strcspn(subscriber->m_name, "\n")] = 0;
    subscriber->m_password[strcspn(subscriber->m_password, "\n")] = 0;

    return subscriber;
}

static char* create_key(char* name)
{
    char* user_name = (char*)malloc(sizeof(char)*strlen(name));
    if (user_name == NULL)
        return NULL;

    strcpy(user_name, name);
    return user_name;
}

static SubscribsManager_return push_subscriber(Subscriber* subscriber, SubscribsManager* subscribs_nanager)
{
	if (hash_map_is_exists(subscribs_nanager->m_subscribs , subscriber->m_name))
        return SUBSCRIBS_MANAGER_USERNAME_ALREADY_EXISTS;

    char* name = create_key(subscriber->m_name);
    if (name == NULL) {return SUBSCRIBS_MANAGER_MALLOC_FAIL;}

    Map_return status = hash_map_insert(subscribs_nanager->m_subscribs, name, subscriber);
    if (status != MAP_SUCCESS)
    {
        free(name);
        free(subscriber);
        return SUBSCRIBS_MANAGER_PUSH_FAIL;
    }
            
    return SUBSCRIBS_MANAGER_SUCCESS;
}

static SubscribsManager_return load_subscribs_from_file(SubscribsManager* subscribs_nanager)
{
    FILE* file;
	if((file = fopen("../resources/subscribs.txt", "r")) == NULL) {return OPEN_FILE_FAIL;}

	while (!feof(file))
	{
		Subscriber* subscriber = create_subscriber_from_file(file, subscribs_nanager);
        if(subscriber == NULL){return SUBSCRIBS_MANAGER_CREATE_FROM_FILE_FAIL;}

        SubscribsManager_return result = push_subscriber(subscriber, subscribs_nanager);
        if(result != SUBSCRIBS_MANAGER_SUCCESS) {return result;}
	}

	fclose(file);
	return SUBSCRIBS_MANAGER_SUCCESS;
}

SubscribsManager* create_subscribs_manager(int capacity)
{
    if(capacity < 1)
        return NULL;

    SubscribsManager* subscribs_nanager = (SubscribsManager*)malloc(sizeof(SubscribsManager));
    if(subscribs_nanager == NULL) {return NULL;}

    subscribs_nanager->m_subscribs = hash_map_create(capacity, hash_for_subscriber_name, compare_subscriber_names, destroy_subscriber_element);
    if(subscribs_nanager->m_subscribs == NULL)
    {
        free(subscribs_nanager);
        return NULL;
    }

    SubscribsManager_return result = load_subscribs_from_file(subscribs_nanager);
     if(result != SUBSCRIBS_MANAGER_SUCCESS) {return NULL;}
    return subscribs_nanager;
}

void destroy_subscribs_manager(SubscribsManager* subscribs_manager)
{
    if(subscribs_manager == NULL)
        return;

    hash_map_destroy(&subscribs_manager->m_subscribs);
    free(subscribs_manager);
    subscribs_manager = NULL;
}

static void encrypt_password(char* encrypted_password, char* password)
{
    SHA256_CTX ctx;
    BYTE buf[SHA256_BLOCK_SIZE];
    
    sha256_init(&ctx);
	sha256_update(&ctx, (BYTE*)password, strlen(password));
	sha256_final(&ctx, buf);

    int j = 0;
    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i)
    { 
        sprintf(encrypted_password + j, "%02x", buf[i]);
        j+=2;
    }
}

static Subscriber* create_subscriber(SubscribsManager* subscribs_nanager, const char* name, char* password)
{
	Subscriber* subscriber = (Subscriber*)malloc(sizeof(Subscriber));
    if(subscriber == NULL) {return NULL;}

    strcpy(subscriber->m_name, name);
    encrypt_password(subscriber->m_password, password);
    return subscriber;
}

static SubscribsManager_return write_subscriber_to_file(Subscriber* subscriber)
{
    FILE* file; 
	if((file = fopen("../resources/subscribs.txt", "a")) == NULL)
		return OPEN_FILE_FAIL;
		
	fprintf(file, "%s\n%s\n", subscriber->m_name, subscriber->m_password);	

	fclose(file);	
	return SUBSCRIBS_MANAGER_SUCCESS;
}

SubscribsManager_return new_subscriber(SubscribsManager* subscribs_manager, const char* name, char* password)
{
    if(subscribs_manager == NULL || name == NULL || password == NULL)
        return SUBSCRIBS_MANAGER_ARGS_NOT_INITIALIZED;
    
    if(hash_map_is_exists(subscribs_manager->m_subscribs, name))
        return SUBSCRIBS_MANAGER_USERNAME_ALREADY_EXISTS;

    Subscriber* subscriber = create_subscriber(subscribs_manager, name, password);
    if(subscriber == NULL) {return SUBSCRIBS_MANAGER_CREATE_SUBSCRIBER_FAIL;}

    SubscribsManager_return result = push_subscriber(subscriber, subscribs_manager);
    if(result != SUBSCRIBS_MANAGER_SUCCESS) {return result;}

    result = write_subscriber_to_file(subscriber);
    if(result == OPEN_FILE_FAIL) {return SUBSCRIBS_MANAGER_WRITE_TO_FILE_FAIL;}

    return SUBSCRIBS_MANAGER_SUCCESS;
}

static int is_password_valid(BYTE* first, BYTE* second)
{
    if (strcmp((char*)first, (char*)second) == 0)
		return TRUE;

	return FALSE;
}

SubscribsManager_return log_in_chack(SubscribsManager* subscribs_manager, const char* name, char* password)
{
    if(subscribs_manager == NULL || name == NULL || password == NULL)
        return SUBSCRIBS_MANAGER_ARGS_NOT_INITIALIZED;

    Subscriber* subscriber;
    if(hash_map_find(subscribs_manager->m_subscribs, name, (void**)&subscriber) == MAP_KEY_NOT_EXISTS)
        return SUBSCRIBS_MANAGER_USERNAME_NOT_EXISTS;

    char encrypted_password[STRING_SIZE];
    encrypt_password(encrypted_password, password);
    if(is_password_valid((BYTE*)subscriber->m_password, (BYTE*)encrypted_password))
        return SUBSCRIBS_MANAGER_SUCCESS;

    return SUBSCRIBS_MANAGER_INVALID_PASSWORD;
}