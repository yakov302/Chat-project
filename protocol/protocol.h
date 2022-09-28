#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "args.h"

/*
    ***buufer order*** 

    [toal message size]      (4 bytes)

    [message type]           (1 bytes)

    [number of strings]      (1 bytes)

    [strings 1 size]         (1 bytes)
    [string 1]
        .
        .
        .
    [strings n size]         (1 bytes)
    [string n]

    [number of ints]         (1 bytes)

    [int 1]
        .
        .
        .
    [int n]
*/

typedef enum Message_type
{
    // Internal messages

    BUFFER_NOT_INITIALIZE,
    ARGS_NOT_INITIALIZE,
    UNKNOWN_COMMAND,

    // Client to server

    REGISTRATION_REQUEST,
    LOG_IN_REQUEST,
    EXIT_CHAT_REQUEST,
    OPEN_NEW_GROUP_REQUEST,
    PRINT_EXISTING_GROUPS_REQUEST,
    JOIN_EXISTING_GROUP_REQUEST,
    LEAVE_GROUP_REQUEST,
    WAKE_UP_CLIENT,

    // Server to client

    REGISTRATION_SUCCESS,
    REGISTRATION_USER_NAME_ALREADY_EXISTS,
    REGISTRATION_FAIL,
    LOG_IN_SUCCESS,
    LOG_IN_FAIL,
    LOG_IN_WRONG_USER_NAME,
    LOG_IN_WRONG_PASSWORD,
    LOG_IN_USER_ALREADY_LOGGED_IN,
    EXIT_CHAT_SUCCESS,
    EXIT_CHAT_USER_NOT_EXISTS,
    EXIT_CHAT_FAIL,
    OPEN_NEW_GROUP_SUCCESS,
    OPEN_NEW_GROUP_FAIL,
    OPEN_NEW_GROUP_GROUP_NAME_ALREADY_EXISTS,
    OPEN_NEW_GROUP_USER_NAME_NOT_EXISTS,
    PRINT_EXISTING_GROUPS_SUCCESS,
    PRINT_EXISTING_GROUPS_NO_GROUPS,
    JOIN_EXISTING_GROUP_SUCCESS,
    JOIN_EXISTING_GROUP_FAIL,
    JOIN_EXISTING_GROUP_USER_ALREADY_CONNECT,
    JOIN_EXISTING_GROUP_GROUP_NOT_EXISTS,
    JOIN_EXISTING_GROUP_USER_NOT_EXISTS,
    LEAVE_GROUP_SUCCESS,
    LEAVE_GROUP_FAIL,
    LEAVE_GROUP_GROUP_DELETED,
    LEAVE_GROUP_GROUP_NOT_EXISTS,
    LEAVE_GROUP_USER_NOT_EXISTS,
    
} Message_type;


int pack(char* buffer, Args* arganmats, Message_type message);

Message_type unpack(char* buffer, Args* arganmats);

int message_size(char* buffer);

Message_type message_type(char* buffer);


#endif //PROTOCOL_H
