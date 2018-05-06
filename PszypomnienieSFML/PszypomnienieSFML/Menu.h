#include "stdafx.h"
#include "SFML\Graphics.hpp"
#include "Engine.h"

using namespace std;
using namespace sf;

float ResX = 1920;
float ResY = 1080;
RenderWindow window(VideoMode(ResX, ResY), "Space Shooter");

class Menu
{
	public:
		Menu();
		~Menu();
		void LaunchMenu();
		void ShowMenu();
		void DrawMenu(Text MenuWelcome, Text GameStart, Text GameEnd);
		void CenterizeMenu();
		Font font;
		Event MenuChoice;
		InGameStatus Status;
};

Menu::Menu()
{
	Status = MENU;
	window.setFramerateLimit(60);
	font.loadFromFile("Data/Fonts/font.ttf");
}

Menu::~Menu()
{
}

void Menu::LaunchMenu()
{
	do
	{
		switch (Status)
		{
		case MENU:
		{
			ShowMenu();
			break;
		}
		case INGAME:
		{
			Status = MENU;
			break;
		}
		default:
			break;
		}
	} while (Status != END);
}

void Menu::ShowMenu()
{
	Text MenuWelcome("Spaceships", font, 55);
	Text GameStart("Start Game", font, 35);
	Text GameEnd("Turn off", font, 35);
	MenuWelcome.setPosition(ResX / 2 - MenuWelcome.getGlobalBounds().width / 2, 50);
	GameStart.setPosition(ResX / 2 - GameStart.getGlobalBounds().width / 2, 250);
	GameEnd.setPosition(ResX / 2 - GameEnd.getGlobalBounds().width / 2, 450);
	MenuWelcome.setColor(Color::White);
	GameStart.setColor(Color::White);
	GameEnd.setColor(Color::White);
	while (Status == MENU)
	{
		Vector2f Mouse(Mouse::getPosition(window));
		DrawMenu(MenuWelcome, GameStart, GameEnd);
		while (window.pollEvent(MenuChoice))
		{
			if (MenuChoice.type == Event::Closed || MenuChoice.type == Event::KeyPressed && MenuChoice.key.code == Keyboard::Escape)
			{
				Status = END;
			}
			if (MenuChoice.type == Event::MouseButtonReleased && MenuChoice.key.code == Mouse::Left && GameEnd.getGlobalBounds().contains(Mouse))
			{
				Status = END;
			}
			if (MenuChoice.type == Event::MouseButtonReleased && MenuChoice.key.code == Mouse::Left && GameStart.getGlobalBounds().contains(Mouse))
			{
				Status = INGAME;
				Engine *LaunchGame = new Engine(window, ResX, ResY);
				LaunchGame->LaunchGame(window, Status, ResX, ResY, font);
				CenterizeMenu();
				Status = MENU;
			}
		}
	}
}

void Menu::DrawMenu(Text MenuWelcome, Text GameStart, Text GameEnd)
{
	Vector2f Mouse(Mouse::getPosition(window));
	if (GameStart.getGlobalBounds().contains(Mouse))
	{
		GameStart.setColor(Color::Red);
	}
	else
	{
		GameStart.setColor(Color::White);
	}
	if (GameEnd.getGlobalBounds().contains(Mouse))
	{
		GameEnd.setColor(Color::Red);
	}
	else
	{
		GameEnd.setColor(Color::White);
	}
	window.clear();
	window.draw(MenuWelcome);
	window.draw(GameStart);
	window.draw(GameEnd);
	window.display();
}

void Menu::CenterizeMenu()
{
	View NewView;
	NewView.setSize(ResX, ResY);
	NewView.setCenter(ResX / 2, ResY / 2);
	window.setView(NewView);
}