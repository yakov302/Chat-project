#include "protocol.h"

#define NOT_INITIALIZE -1
#define COMPLETE_MESSAGE 1
#define INCOMPLETE_MESSAGE 0

static void EncryptBuffer(char _encryptionKey[], void* _buffer, int _messageSize);
static void DecryptBuffer(char _encryptionKey[], void* _encryptBuffer, int _messageSize);

int PackFirstAndSecond(FirstAndSecond* _struct, void* _buffer, MessagesTypes _messagesTypes)
{
	char* pack = (char*) _buffer;
	int m_secondLen;
	int m_firstLen;
	
	if(_struct == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
			
	m_firstLen = strlen(_struct -> m_first);
	m_secondLen = strlen(_struct -> m_second);
	
	*pack = _messagesTypes;
	*(pack + 1) = m_firstLen + m_secondLen + 2;
	*(pack + 2) = m_firstLen;	
	strcpy((pack + 3), _struct -> m_first);
	*(pack + 3 + m_firstLen) = m_secondLen;
	strcpy((pack + 4 + m_firstLen), _struct -> m_second);

	EncryptBuffer("YakovYosiRotem", pack, m_firstLen + m_secondLen + 4);

	_buffer = pack;

	return m_firstLen + m_secondLen + 4;
}

MessagesTypes UnpackFirstAndSecond(FirstAndSecond* _struct, void* _buffer, int _messageSize)
{
	int m_firstLen;
	int m_secondLen;
	char* pack = (char*) _buffer;

	if(_struct == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
	
	DecryptBuffer("YakovYosiRotem", _buffer, _messageSize);
			
	m_firstLen = *(pack + 2);
	m_secondLen = *(pack + 3 + m_firstLen);
	
	strncpy(_struct -> m_first, (pack + 3), m_firstLen); 
	_struct -> m_first[m_firstLen] = '\0';
	strncpy(_struct -> m_second, (pack + 4 + m_firstLen), m_secondLen);
	_struct -> m_second[m_secondLen] = '\0';
	
	return *pack;
}

int PackStringMassage(char _str[] , void* _buffer, MessagesTypes _messagesTypes)
{
	char* pack = (char*) _buffer;
	int strLen;

	if(_str == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
			
	strLen = strlen(_str);

	*pack = _messagesTypes;
	*(pack + 1) = strLen;
	strcpy((pack + 2), _str);
	
	EncryptBuffer("YakovYosiRotem", pack, strLen + 2);

	_buffer = pack;

	return strLen + 2;
}

MessagesTypes UnpackStringMassage(char _str[], void* _buffer, int _messageSize)
{
	if(_str == NULL || _buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
	
	DecryptBuffer("YakovYosiRotem", _buffer, _messageSize);
	
	char* pack = (char*) _buffer;
	int strLen = *(pack + 1);
			
	strncpy(_str, (pack + 2), strLen); 
	*(_str + strLen + 2) = '\0';
		
	return *pack;
}

int PackStatusMassage(void* _buffer, MessagesTypes _messagesTypes)
{
	char* pack = (char*) _buffer;

	if(_buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
		
	*pack = _messagesTypes;
	*(pack + 1) = 0;

	EncryptBuffer("YakovYosiRotem", pack, 2);

	_buffer = pack;

	return 2;
}

MessagesTypes UnpackStatusMassage(void* _buffer, int _messageSize)
{
	char* pack = (char*) _buffer;

	if(_buffer == NULL)
	{
		return NOT_INITIALIZE;	
	}
	
	DecryptBuffer("YakovYosiRotem", _buffer, _messageSize);
	
	pack = (char*) _buffer;
		
	return *pack;
}

int ReturnMessageSize(void* _buffer)
{
	char* unpack;
	int size;

	DecryptBuffer("YakovYosiRotem", _buffer, 2);
	unpack = (char*) _buffer;
	size = *(unpack + 1) + 2;
	EncryptBuffer("YakovYosiRotem", _buffer, 2);

	return size;
}

MessagesTypes ReturnMessageType(void* _buffer)
{
	char* unpack;
	MessagesTypes type;

	DecryptBuffer("YakovYosiRotem", _buffer, 2);
	unpack = (char*) _buffer;
	type = *(unpack);
	EncryptBuffer("YakovYosiRotem", _buffer, 2);

	return type;
}

int IsThatTheWholeMessage (char _encryptionKey[], void* _buffer, int _messageSize)
{
	char* pack = (char*) _buffer;
	int totalLen;
	int strLen;

	DecryptBuffer("YakovYosiRotem", _buffer, 3);
	pack = (char*) _buffer;
	totalLen = *(pack + 1) + 2;
	EncryptBuffer("YakovYosiRotem", _buffer, 3);

	if(totalLen == _messageSize)
	{
		return COMPLETE_MESSAGE;
	}
	
	return INCOMPLETE_MESSAGE;	
}

static void EncryptBuffer(char _encryptionKey[], void* _buffer, int _messageSize)
{
	char* encryptBuffer = (char*) _buffer;
	int keyLen = strlen(_encryptionKey);
	int j = 0;
	int i;

	for(i = 0; i < _messageSize; i++)
	{
		*(encryptBuffer + i) =  *(encryptBuffer + i) + _encryptionKey[j];
		j = (j + 1)%keyLen;
	}
	
	_buffer = encryptBuffer;
}

static void DecryptBuffer(char _encryptionKey[], void* _encryptBuffer, int _messageSize)
{
	char* decryptBuffer = (char*) _encryptBuffer;
	int keyLen = strlen(_encryptionKey);
	int j = 0;
	int i;

	for(i = 0; i < _messageSize; i++)
	{
		*(decryptBuffer + i) = *(decryptBuffer + i) - _encryptionKey[j];
		j = (j + 1)%keyLen;
	}
	
	_encryptBuffer = decryptBuffer;
}


