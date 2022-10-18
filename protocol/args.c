#include "args.h"

static void malloc_strings(char** m_strings, int num_of_strings)
{
    if(num_of_strings < 1)
    {
        *m_strings = NULL;
    }
    else
    {
        *m_strings = (char*)malloc(STRING_SIZE*sizeof(char)*num_of_strings);
        if(*m_strings == NULL)
            perror("malloc m_strings fail: \n");
    }
}

static void malloc_ints(int** m_ints, int num_of_ints)
{
    if(num_of_ints < 1)
    {
        *m_ints = NULL;
    }
    else
    {
        *m_ints = (int*)malloc(num_of_ints*sizeof(int));
        if(*m_ints == NULL)
            perror("malloc m_ints fail: \n");
    }
}

static void malloc_floats(float** m_floats, int num_of_floats)
{
    if(num_of_floats < 1)
    {
        *m_floats = NULL;
    }
    else
    {
        *m_floats = (float*)calloc(num_of_floats, sizeof(float));
        if(*m_floats == NULL)
            perror("malloc m_ints fail: \n");
    }
}

void args_create(Args* args, int num_of_strings, int num_of_ints, int num_of_float)
{
    if(args == NULL)
        return;

    args->m_num_of_strings = 0;
    args->m_num_of_ints = 0;
    args->m_num_of_floats = 0;

    if(num_of_strings < 1 && num_of_ints < 1 && num_of_float < 1)
    {
        args->m_strings = NULL;
        args->m_ints = NULL;
        args->m_floats = NULL;
        return;
    }

    malloc_strings(&args->m_strings, num_of_strings);
    malloc_ints(&args->m_ints, num_of_ints);
    malloc_floats(&args->m_floats, num_of_float);
}

char* strings(Args* args)
{
    return args->m_strings;
}

char* strings_by_index(Args* args, int index)
{
    return (args->m_strings + index*STRING_SIZE);
}

int* ints(Args* args)
{
    return args->m_ints;
}

float* floats(Args* args)
{
    return args->m_floats;
}

int num_of_strings(Args* args)
{
    return args->m_num_of_strings;
}

int num_of_ints(Args* args)
{
    return args->m_num_of_ints;
}

int num_of_floats(Args* args)
{
    return args->m_num_of_floats;
}

void push_string(Args* args, const char* string)
{
    if(args == NULL || args->m_strings == NULL || string == NULL)
        return;

    strcpy(args->m_strings + (args->m_num_of_strings*STRING_SIZE), string);
    ++args->m_num_of_strings;
}

void push_string_by_len(Args* args, const char* string, int str_size)
{
    strncpy(args->m_strings + (args->m_num_of_strings*STRING_SIZE), string, str_size);
    *(args->m_strings + (args->m_num_of_strings*STRING_SIZE) + str_size) = '\0';
    ++args->m_num_of_strings;
}

void push_int(Args* args, int number)
{
    if(args == NULL || args->m_ints == NULL)
        return;

    args->m_ints[args->m_num_of_ints] = number;
    ++args->m_num_of_ints;
}

void push_float(Args* args, float number)
{
    if(args == NULL || args->m_floats == NULL)
        return;

    args->m_floats[args->m_num_of_floats] = number;
    ++args->m_num_of_floats;
}

void args_destroy(Args* args)
{
    if(args == NULL)
        return;
        
    if(args->m_strings != NULL)
    {
        free(args->m_strings);
        args->m_strings = NULL;
    }

    if(args->m_ints != NULL)
    {
        free(args->m_ints);
        args->m_ints = NULL;
    }

    if(args->m_floats != NULL)
    {
        free(args->m_floats);
        args->m_floats = NULL;
    }
}
