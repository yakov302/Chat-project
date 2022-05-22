#include "ui.h"

#define YELLOW "\x1B[33m"
#define NORMAL "\x1B[0m"
#define RED "\x1B[91m"
#define GREEN "\x1B[32m"

/********************global var*********/

int mainChoice = 0;
int secondChoice = 0;

/*********************MainMenu************************/

int MainMenu(void)
{
	printf(YELLOW);
	printf("\n--------------Y.Y.R chat--------------\n\n");
	printf(NORMAL);
	printf("1 - Registration\n");
	printf("2 - Log in\n");
	printf("3 - Exit\n");
	printf( "Select your choice: ");
	scanf("%d", &mainChoice);
	
	return mainChoice;
}

/************************SecondMenu**********************/

int SecondMenu(void)
{
	printf(YELLOW);
	printf("\n--------------Y.Y.R chat--------------\n\n");
	printf(NORMAL);
	printf("1 - Create new group\n");
	printf("2 - Print existing groups\n");
	printf("3 - Join existing group\n");
	printf("4 - Leave existing group\n");
	printf("5 - Log out\n");
	printf("Select your choice: ");
	scanf("%d", &secondChoice);
	
	return secondChoice;
}

/********************InsertName***********************/

char* InsertName(char _name[])
{
	printf("please enter your user name: ");
	scanf("%s", _name);  
	return _name;
}

/********************InsertGroupName***********************/

void InsertGroupName(char _name[])
{
	printf("please enter group name: ");
	scanf("%s", _name);  
	return;
}

/*********************InsertPassWord**************************/

char* InsertPassWord(char _passWord[])
{
	printf("please enter your passWord: ");
	scanf("%s", _passWord);  
	return _passWord;
}

/*********************PrintInvalidChoice*************************/

void PrintInvalidChoice(void)
{
	printf("Invalid choice!\n");
} 

/*********************PrintToClient*************************/

void PrintToClient(MessagesTypes _type)
{	
	switch(_type)
	{
		case REGISTRATION_REQUEST_SUCCESS:
			printf(GREEN);
			printf("\n  ->  ");
			printf("Registration success!");
			printf("  <-  \n");
			break;
			
		case REGISTRATION_REQUEST_DUPLICATE_USERNAME:
			printf(RED);
			printf("\n  ->  ");
			printf("Registration fail. Duplicate user name!");
			printf("  <-  \n");
			break;
				
		case REGISTRATION_REQUEST_FAIL:
			printf(RED);
			printf("\n  ->  ");
			printf("Registration fail. try again!");
			printf("  <-  \n");
			break;
			
		case LOG_IN_REQUEST_SUCCESS:
			printf(GREEN);
			printf("\n  ->  ");
			printf("Log in success!");
			printf("  <-  \n");
			break;
				
		case LOG_IN_REQUEST_WRONG_DETAILS:
			printf(RED);
			printf("\n  ->  ");
			printf("Log in fail. Wrong details!");
			printf("  <-  \n");
			break;
				
		case LOG_IN_REQUEST_FAIL:
			printf(RED);
			printf("\n  ->  ");
			printf("Log in fail. Try again!");
			printf("  <-  \n");
			break;

		case LEAVE_CHAT_SUCCESS:
			printf(GREEN);
			printf("\n  ->  ");
			printf("Successfully disconnected!");
			printf("  <-  \n");
			break;
		
		case OPEN_NEW_GROUP_SUCCESS:
			printf(GREEN);
			printf("\n  ->  ");
			printf("The group opened successfully!");
			printf("  <-  \n");
			break;	
		
		case OPEN_NEW_GROUP_FAIL:
			printf(RED);
			printf("\n  ->  ");
			printf("Failed to open the group. try again!");
			printf("  <-  \n");
			break;	
			
		case DUPLICATE_GROUP_NAME:
			printf(RED);
			printf("\n  ->  ");
			printf("Duplicate group name. choose new name!");
			printf("  <-  \n");
			break;
			
		case JOIN_EXISTING_GROUP_SUCCESS:
			printf(GREEN);
			printf("\n  ->  ");
			printf("successfully connected to the group!");
			printf("  <-  \n");
			break;
			
		case JOIN_EXISTING_GROUP_FAIL:
			printf(RED);
			printf("\n  ->  ");
			printf("Join fail. try again!");
			printf("  <-  \n");
			break;

		case DUPLICATE_GROUP_CONNECT:
			printf(RED);
			printf("\n  ->  ");
			printf("You are already in the group");
			printf("  <-  \n");
			break;			

		case GROUP_NOT_FOUND:
			printf(RED);
			printf("\n  ->  ");
			printf("No such group. try again!");
			printf("  <-  \n");
			break;	
			
		case LEAVE_GROUP_SUCCESS:
			printf(GREEN);
			printf("\n  ->  ");
			printf("You left the group successfully!");
			printf("  <-  \n");
			break;
			
		case LEAVE_GROUP_FAIL:
			printf(RED);
			printf("\n  ->  ");
			printf("Group leaving failed. try again!");
			printf("  <-  \n");
			break;

		case GROUP_DELETED:
			printf(GREEN);
			printf("\n  ->  ");
			printf("You last. The group deleted!");
			printf("  <-  \n");
			break;					
	}
}

/*********************printGroupsNames*************************/

void printGroupsNames(char _str[])
{
	printf("%s", _str);
}

void printGroups(char _str[])
{
	printf("\nExisting groups:\n");
	printf("%s", _str);	
}

