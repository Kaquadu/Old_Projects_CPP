#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <Windows.h>
#include <fstream>
#include <time.h>
#include <cstdio>
#include <string>
#include "Game.h"

using namespace std;
using namespace sf;


int main()
{
	srand(time(NULL));
	Game Session;
	Session.RunGame();
	return 0;
}