#include "groups_manager.h"

static void FillTheIpDatabase(Queue* _queue);
static void DestroyGroupGen (void* _group);
static void DestroyIp (void* _ip);
static void DestroyGroupName (void* _groupName);
static void printKey(void* _word);
static void printValue(void* _value);

size_t ConvertGroupNameForHash (void* _key)
{
	int i;
	char letter;
	size_t multi = 1;
	int len = strlen ((char*)_key);

	for (i=0; i< len; i++)
	{
		letter = ((char*)_key)[i];
		multi *= letter;
	}
	
	return multi;
}

int CompareGroupsNames (void* _firstKey, void* _secondKey)	
{
	if (strcmp ((char*)_firstKey, (char*)_secondKey) == 0)
	{
		return EQUAL;
	}
	
	return NOT_EQUAL;
}

GrupsMng* CreateGroupsManager (int _capacity)
{
	GrupsMng* manager;
	HashMap* map;
	Queue* queue;

	manager = (GrupsMng*) malloc (sizeof (GrupsMng));
	if (manager == NULL)
	{return NULL;}
	
	map = HashMapCreate(_capacity, ConvertGroupNameForHash, CompareGroupsNames);
	if (map == NULL)
	{
		free(manager);
		return NULL; 
	}
		
	queue = QueueCreate(QUEUE_SIZE);
	if (queue == NULL)
	{
		HashMapDestroy(&map, DestroyGroupName, DestroyGroupGen);
		free(manager);
		return NULL; 
	}
	manager -> m_groupDatabase = map;
	manager -> m_ipDatabase = queue;	

	FillTheIpDatabase(manager -> m_ipDatabase);
	return manager;
}

static int printQueue (const void* _element, void* _context) // for debug
{
	printf ("%s\n", (char*)_element);
	return 1;
}

static void FillTheIpDatabase(Queue* _queue)
{
	int i;
	void* ip;
	char ipTemp [IP_SIZE] = "224.255.255.";
	char buf[IP_SIZE];

	for(i = 1; i < QUEUE_SIZE; i++)
	{
		if ((ip = (void*) malloc (sizeof (void*))) == NULL)
		{return;}
		sprintf(buf, "%d", i);
		strcpy(ip, ipTemp);
		strcat(ip, buf);
		QueueInsert(_queue, ip);	
	}
}

void DestroyGroupsManager (GrupsMng* _manager)
{
	HashMap* map;
	Queue* queue;

	if ( _manager == NULL ||  _manager -> m_magicNumber != MAGIC_NUMBER)
	{return;}
	
	_manager -> m_magicNumber = 0;

	map = (HashMap*)_manager -> m_groupDatabase;
	queue = (Queue*) _manager -> m_ipDatabase;
	HashMapDestroy(&map, DestroyGroupName, DestroyGroupGen);
	QueueDestroy(&queue, DestroyIp);	
	free (_manager);
}

GrupsMngResult CreateGroup (char _groupName[] , GrupsMng* _manager, char _buffer[])
{
	GrupsMngResult result;
	Group* group;
	void* pValue;
	char* groupName;
	char* ip;
	int status;
	int len;

	if (_manager == NULL)
	{return MANAGER_NOT_INITIALIZED;}
	
	result = HashMapFind(_manager -> m_groupDatabase , _groupName , &pValue);
	if (result == MAP_KEY_NOT_FOUND_ERROR)
	{	
		len = strlen (_groupName);
		groupName = (char*) malloc (sizeof(char)* len);
		if (groupName == NULL) 
		{return GROUP_NOT_INITIALIZED;}

		strcpy (groupName, _groupName);
		group = CreateNewGroup (_groupName);
		if (group == NULL)
		{
			free (groupName);
			return GROUP_NOT_INITIALIZED;
		}

		status = HashMapInsert( _manager->m_groupDatabase, groupName, group);
		//HashMapPrint(_manager -> m_groupDatabase, printKey ,printValue ); for debug
		if (status != MAP_SUCCESS)
		{
			DestroyGroup (group);
			free (groupName);
			return HASH_MAP_INSERT_FAIL;
		}
		
		QueueRemove(_manager -> m_ipDatabase , (void*)&ip);
		SetGroupIp (group, ip);
		strcpy (_buffer, ip);
		IncreseNumOfCleints (group);
		return GROUP_SUCCESS;
	}
	
	return DUPLICATE_GROUP_NAME_FAIL;
}

GrupsMngResult JoinExistingGroup (char _groupName[], GrupsMng* _manager, char _buffer[])
{
	MapResult result;
	void* pValue;

	if (_manager == NULL)
	{return MANAGER_NOT_INITIALIZED;}
	
	result = HashMapFind(_manager->m_groupDatabase , _groupName , &pValue);
	if (result == MAP_KEY_NOT_FOUND_ERROR)
	{return GROUP_NOT_FOUND_IN_HASH;}
	GetGroupIp ((Group*)pValue, _buffer);
	IncreseNumOfCleints ((Group*)pValue );
	
	return GROUP_SUCCESS;
}

GrupsMngResult LeaveGroup (char _groupName[], GrupsMng* _manager, char _buffer[])
{
	void* pValue;
	void* pKey;
	MapResult result;

	if (_manager == NULL)
	{return MANAGER_NOT_INITIALIZED;}
	
	result = HashMapFind(_manager -> m_groupDatabase , _groupName, &pValue);
	if (result == MAP_KEY_NOT_FOUND_ERROR)
	{return GROUP_NOT_FOUND_IN_HASH;}
	
	DecreseNumOfCleints ((Group*)pValue);
	if(GetGroupNumOfClients((Group*)pValue) == 0)
	{
		result = HashMapRemove(_manager->m_groupDatabase, _groupName, &pKey, &pValue);
		DestroyGroup((Group*)pValue);
		return GROUP_DELETE;
	}

	return GROUP_SUCCESS;
}

static void DestroyGroupGen (void* _group) 
{
	DestroyGroup ((Group*) _group);
}

static void DestroyIp (void* _ip) 
{
	free(_ip);
}

static void DestroyGroupName (void* _groupName) 
{
	free(_groupName);
}

static void printKey(void* _word)
{
	
	printf("Key: %s\n", ((char*)_word));
}

static void printValue(void* _value)
{
	
}

static void PullKey2(void* _data, char _str[])
{
	strcat(_str, (char*)_data);
	strcat(_str, "\n");
}

void giveGroups(GrupsMng* _groups, char _str[])
{
	PutKeysInStr(_groups -> m_groupDatabase, _str , PullKey2);
	_str[strlen(_str)] = '\0';
}



