#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdio>
#include <string>
#include <fstream>
#include <Windows.h>
#include "Engine.h"

using namespace std;
using namespace sf;

RenderWindow window(VideoMode(1024, 720), "4 W Linii", Style::Resize);


class Game
{
public:
	Game();
	~Game();
	void RunGame();
	void ShowMenu();
	void Instruction();
	Font font;
	void Singleplayer();
	void Multiplayer();
	GameStatus getSessionStatus() {return SessionStatus;};
	Event OurChoose;
	bool Mode;
	void Statystyka();
	int iloscwygranych[2];
private:
	GameStatus SessionStatus;
};

Game::Game()
{
	SessionStatus = END;
	if (!font.loadFromFile("debug/FFF_Tusj.ttf"))
	{
		MessageBox(NULL, "Font not found!", "ERROR", NULL);
		return;
	}
	SessionStatus = MENU;
}

Game::~Game()
{
	SessionStatus = GAME_OVER;
}

void Game::RunGame()
{
	while (SessionStatus != END)
	{
		switch (SessionStatus)
		{
		case MENU:
			{
				ShowMenu();
				break;
			}
		case GAME_SP:
			{
				Singleplayer();
				break;
			}
		case GAME_MP:
			{
				Multiplayer();
				break;
			}
		case GAME_OVER:
			{
				SessionStatus = MENU;
				break;
			}		
		}
	}
}

void Game::ShowMenu()
{
	Text title("4 in a row", font, 150);
	title.setPosition(1024 / 2 - title.getGlobalBounds().width / 2, 20);

	const int Much = 5;
	Text Choose[Much];

	String MenuChooses[] = { "2 Players", "vs AI", "Instruction","Stats" , "Exit", };

	for (int i = 0; i < Much; i++)
	{
		Choose[i].setFont(font);
		Choose[i].setCharacterSize(45);
		Choose[i].setString(MenuChooses[i]);
		Choose[i].setPosition(1024 / 2 - Choose[i].getGlobalBounds().width / 2, 200+(i*100));
	}

	while (SessionStatus == MENU)
	{
		Vector2f MousePos(Mouse::getPosition(window));
		for (int i = 0; i < Much; i++)
		{
			if (Choose[i].getGlobalBounds().contains(MousePos))
			{
				Choose[i].setColor(Color::Red);
			}
			else Choose[i].setColor(Color::White);
		}
		window.clear();
		window.draw(title);
		for (int i = 0; i < Much; i++)
		{
			window.draw(Choose[i]);
		}
		window.display();
		while (window.pollEvent(OurChoose))
		{
			if (OurChoose.type == Event::Closed || OurChoose.type == Event::KeyPressed && OurChoose.key.code == Keyboard::Escape)
			{
				SessionStatus = END;
			}
			else if (Choose[4].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
			{
				SessionStatus = END;
			}
			else if (Choose[2].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
			{
				SessionStatus = INSTR;
				Instruction();
				SessionStatus = MENU;
			}
			else if (Choose[0].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
			{
				SessionStatus = GAME_MP;
				Multiplayer();
				SessionStatus = MENU;
			}
			else if (Choose[1].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
			{
				SessionStatus = GAME_SP;
				Singleplayer();
				SessionStatus = MENU;
			}
			else if (Choose[3].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
			{
				SessionStatus = STATS;
				Statystyka();
				SessionStatus = MENU;
			}
		}
	}
}

void Game::Instruction()
{
	bool Return = false;
	while (Return == false)
	{
		Vector2f MousePosx(Mouse::getPosition(window));
		window.clear();
		Event InstrEnd;
		Text InstrTitle("Instruction!", font, 100);
		InstrTitle.setPosition(1024 / 2 - InstrTitle.getGlobalBounds().width / 2, 20);
		Text InstructionText("You have to arrange four circles in one", font, 30);
		Text InstructionText4("vertical, horizontal or diagonal row.", font, 30);
		Text InstructionText3("The circles are dropping vertically.", font, 30);
		Text InstructionText2("Press here to come back", font, 35);
		InstructionText.setPosition(1024 / 2 - InstructionText.getGlobalBounds().width / 2, 200);
		InstructionText2.setPosition(1024 / 2 - InstructionText2.getGlobalBounds().width / 2, 630);
		InstructionText3.setPosition(1024 / 2 - InstructionText3.getGlobalBounds().width / 2, 300);
		InstructionText4.setPosition(1024 / 2 - InstructionText4.getGlobalBounds().width / 2, 250);
		if (InstructionText2.getGlobalBounds().contains(MousePosx))
		{
			InstructionText2.setColor(Color::Red);
		}
		else InstructionText2.setColor(Color::White);
		window.draw(InstrTitle);
		window.draw(InstructionText);
		window.draw(InstructionText2);
		window.draw(InstructionText4);
		window.draw(InstructionText3);
		window.display();
		while (window.pollEvent(InstrEnd))
			{
				if (Event::KeyPressed && InstrEnd.key.code == Keyboard::BackSpace)
				{
					Return = true;
				}
				else if (InstructionText2.getGlobalBounds().contains(MousePosx) && InstrEnd.type == Event::MouseButtonReleased && InstrEnd.key.code == Mouse::Left)
				{
					Return = true;
				}
			}
	}
}


void Game::Singleplayer()
{
	bool Choose = false;
	while (Choose == false)
	{
		Vector2f MousePos(Mouse::getPosition(window));
		window.clear();
		Event NewChoose;
		Text Hardmode("Hard", font, 35);
		Text Easymode("Easy", font, 35);
		Hardmode.setPosition(1024 / 2 - Hardmode.getGlobalBounds().width / 2, 230);
		Easymode.setPosition(1024 / 2 - Easymode.getGlobalBounds().width / 2, 300);
		if (Hardmode.getGlobalBounds().contains(MousePos))
		{
			Hardmode.setColor(Color::Red);
		}
		else Hardmode.setColor(Color::White);
		if (Easymode.getGlobalBounds().contains(MousePos))
		{
			Easymode.setColor(Color::Red);
		}
		else Easymode.setColor(Color::White);
		window.draw(Hardmode);
		window.draw(Easymode);
		window.display();
		while (window.pollEvent(NewChoose))
		{
			if (Hardmode.getGlobalBounds().contains(MousePos) && NewChoose.type == Event::MouseButtonReleased && NewChoose.key.code == Mouse::Left)
			{
				Mode = false;
				Choose = true;
				Engine *NewGame;
				NewGame = new Engine(Mode, window, SessionStatus);
				delete NewGame;
			}
			else if (Easymode.getGlobalBounds().contains(MousePos) && NewChoose.type == Event::MouseButtonReleased && NewChoose.key.code == Mouse::Left)
			{
				Mode = true;
				Choose = true;
				Engine *NewGame;
				NewGame = new Engine(Mode, window, SessionStatus);
				delete NewGame;
			}
		}
	}

	SessionStatus = MENU;
}

void Game::Multiplayer()
{

	Engine *NewGame;
	NewGame = new Engine(window, SessionStatus);
	delete NewGame;

	SessionStatus = MENU;
}


void Game::Statystyka()
{
	ifstream plik;
	plik.open("dane.txt", ios::app);
	//cout << "Otwarty plik" << endl;
	string line;
	int x = 0;
	while (getline(plik, line))
	{
		iloscwygranych[x] = stoi(line);
		//cout << line << " " << iloscwygranych[x];
		x++;
	}
	plik.close();
	bool Return = false;
	while (Return == false)
	{
		Vector2f MousePosx(Mouse::getPosition(window));
		window.clear();
		Event InstrEnd;
		Text InstructionText2("Press here to come back", font, 35);
		Text Zolty("Ilosc wygranych zoltego to:", font, 40);
		Text Zolty2(to_string(iloscwygranych[0]), font, 40);
		Text Czerwony("Ilosc wygranych czwerwonego to:", font, 40);
		Text Czerwony2(to_string(iloscwygranych[1]), font, 40);
		Zolty.setPosition(1024 / 2 - Zolty.getGlobalBounds().width / 2, 100);
		Zolty2.setPosition(1024 / 2 - Zolty2.getGlobalBounds().width / 2, 150);
		Czerwony2.setPosition(1024 / 2 - Czerwony2.getGlobalBounds().width / 2, 250);
		Czerwony.setPosition(1024 / 2 - Czerwony.getGlobalBounds().width / 2, 200);
		InstructionText2.setPosition(1024 / 2 - InstructionText2.getGlobalBounds().width / 2, 630);
		if (InstructionText2.getGlobalBounds().contains(MousePosx))
		{
			InstructionText2.setColor(Color::Red);
		}
		else InstructionText2.setColor(Color::White);
		window.draw(InstructionText2);
		window.draw(Zolty);
		window.draw(Zolty2);
		window.draw(Czerwony);
		window.draw(Czerwony2);
		window.display();
		while (window.pollEvent(InstrEnd))
		{
			if (Event::KeyPressed && InstrEnd.key.code == Keyboard::BackSpace)
			{
				Return = true;
			}
			else if (InstructionText2.getGlobalBounds().contains(MousePosx) && InstrEnd.type == Event::MouseButtonReleased && InstrEnd.key.code == Mouse::Left)
			{
				Return = true;
			}
		}
	}
}