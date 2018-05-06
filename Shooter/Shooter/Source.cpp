#include <SFML/Graphics.hpp>
#include <time.h>
#include <conio.h>
#include "Menu.h"

using namespace std;
using namespace sf;

int main()
{
	srand(time(NULL));
	Game SessionStart;
	SessionStart.RunGame();
	return 0;
}