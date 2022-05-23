#include <stdio.h>
#include <stdlib.h>

#include "client_app.h"
#include "protocol.h"
#include "ui.h"


int resFromMenu = 0;

int main()
{
	resFromMenu = MainMenu();
	RunMainMenu(resFromMenu); 	
}
