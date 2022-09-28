#include "protocol.h"

static void encrypt(const char* key, char* buffer, int message_size)
{
	int key_size = strlen(key);
    int j = 0;

	for(int i = 0; i < message_size; i++)
	{
		*(buffer + i) += key[j];
		j = (j + 1)%key_size;
	}	
}

static void decrypt(const char* key, char* buffer, int message_size)
{
	int key_size = strlen(key);
	int j = 0;

	for(int i = 0; i < message_size; i++)
	{
		*(buffer + i) -= key[j];
		j = (j + 1)%key_size;
	}	
}

static void pack_strings(char* buffer, Args* arganmats, int* num_of_bytes)
{
    int num_of_str = num_of_strings(arganmats);
    *(buffer + *num_of_bytes) = num_of_str;
    ++(*num_of_bytes);

    for(int i = 0; i < num_of_str; ++i)
    {
        int str_size = strlen(strings_by_index(arganmats, i));
        *(buffer + *num_of_bytes) = str_size;
        ++(*num_of_bytes);

        strncpy(buffer + *num_of_bytes, strings_by_index(arganmats, i), str_size);
        *num_of_bytes += str_size;
    }
}

static void pack_ints(char* buffer, Args* arganmats, int* num_of_bytes)
{
    int num_of_int = num_of_ints(arganmats);
    *(buffer + *num_of_bytes) = num_of_int;
    ++(*num_of_bytes);

    for(int i = 0; i < num_of_int; ++i)
    {
        *(int*)(buffer + *num_of_bytes) = ints(arganmats)[i];
        *num_of_bytes += sizeof(int); 
    }
}

static void pack_floats(char* buffer, Args* arganmats, int* num_of_bytes)
{
    int num_of_float = num_of_floats(arganmats);
    *(buffer + *num_of_bytes) = num_of_float;
    ++(*num_of_bytes);

    for(int i = 0; i < num_of_float; ++i)
    {
        *(float*)(buffer + *num_of_bytes) = floats(arganmats)[i];
        *num_of_bytes += sizeof(float); 
    }
}

int pack(char* buffer, Args* arganmats, Message_type message)
{
    if(buffer == NULL) {return BUFFER_NOT_INITIALIZE;}
    if(arganmats == NULL) {return ARGS_NOT_INITIALIZE;}

    int num_of_bytes = sizeof(int); 
    *(buffer + num_of_bytes) = message;
    ++num_of_bytes;

    pack_strings(buffer, arganmats, &num_of_bytes);
    pack_ints(buffer, arganmats, &num_of_bytes);
    pack_floats(buffer, arganmats, &num_of_bytes);

    *(int*)buffer = num_of_bytes;
    encrypt("zaidenberg", buffer, num_of_bytes);
    return num_of_bytes;
}

static void unpack_strings(char* buffer, Args* arganmats, int num_of_strings, int* num_of_bytes)
{
    for(int i = 0; i < num_of_strings; ++i)
    {
        int str_size = *(buffer + *num_of_bytes);
        ++(*num_of_bytes);

        push_string_by_len(arganmats, buffer + *num_of_bytes, str_size); 
        *num_of_bytes += str_size;
    }
}

static void unpack_ints(char* buffer, Args* arganmats, int num_of_ints, int* num_of_bytes)
{
    for(int i = 0; i < num_of_ints; ++i)
    {
        push_int(arganmats, *(int*)(buffer + *num_of_bytes));
        *num_of_bytes += sizeof(int);
    }
}

static void unpack_floats(char* buffer, Args* arganmats, int num_of_floats, int* num_of_bytes)
{
    for(int i = 0; i < num_of_floats; ++i)
    {
        push_float(arganmats, *(float*)(buffer + *num_of_bytes));
        *num_of_bytes += sizeof(float);
    }
}

Message_type unpack(char* buffer, Args* arganmats)
{
    if(buffer == NULL) {return BUFFER_NOT_INITIALIZE;}
    if(arganmats == NULL) {return ARGS_NOT_INITIALIZE;}

    decrypt("zaidenberg", buffer, message_size(buffer));

    int num_of_bytes = sizeof(int);  
    Message_type message = (Message_type)(*(buffer + num_of_bytes));
    ++num_of_bytes;

    int num_of_strings = *(buffer + num_of_bytes);
    ++num_of_bytes;

    if(num_of_strings != 0)
        unpack_strings(buffer, arganmats, num_of_strings, &num_of_bytes);

    int num_of_ints = *(buffer + num_of_bytes);
    ++num_of_bytes;

    if(num_of_ints != 0)
        unpack_ints(buffer, arganmats, num_of_ints, &num_of_bytes);

    int num_of_floats = *(buffer + num_of_bytes);
    ++num_of_bytes;

    if(num_of_floats != 0)
        unpack_floats(buffer, arganmats, num_of_floats, &num_of_bytes);

    return message;
}

int message_size(char* buffer)
{
    if(buffer == NULL)
        return BUFFER_NOT_INITIALIZE;
        
    decrypt("zaidenberg", buffer, sizeof(int));
    int size = *(int*)(buffer);
    encrypt("zaidenberg", buffer, sizeof(int));
    return size;
}

Message_type message_type(char* buffer)
{
    if(buffer == NULL)
        return BUFFER_NOT_INITIALIZE;

    decrypt("zaidenberg",buffer, sizeof(int) + 1);
    Message_type type = (Message_type)*(buffer + sizeof(int));
    encrypt("zaidenberg", buffer, sizeof(int) + 1);
    return type;
}

