#ifndef __UI_H__
#define __UI_H__

int MainMenu(void);

int SecondMenu(void);

char* InsertName(char _name[]);

void InsertGroupName(char _name[]);

char* InsertPassWord(char _passWord[]);

void PrintInvalidChoice(void);

void PrintToClient(MessagesTypes _type);



#endif 
