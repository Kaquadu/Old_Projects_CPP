#include "stdafx.h"
#include "Menu.h"
#include <time.h>
#include <conio.h>


int main()
{
	srand(time(NULL));
	Menu *Session = new Menu;
	Session->LaunchMenu();
	delete Session;
}