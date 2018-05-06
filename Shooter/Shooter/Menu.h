#include <SFML/Graphics.hpp>
#include <time.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include "Scripts.h"

using namespace std;
using namespace sf;

RenderWindow window(VideoMode(1024, 720), "Space Shooter", Style::Resize);

class Game
{
public:
	Game();
	~Game();
	void RunGame();
	void Menu();
	void StartTheGame();
	void JumpingBall();
	void Instruction();
	Font font;
	WhereAreWe Status;
	Event WhereToGo;
	Thread Ball;
	Sprite MenuBall;
	Texture texture;
	string BestScore;
};

Game::Game()
	: Ball(&Game::JumpingBall, this)
{
	Status = END;
	font.loadFromFile("Data/font.ttf");
	Status = MENU;
}

Game::~Game()
{
}

void Game::RunGame()
{
	while (Status != END)
	{
		switch (Status)
		{
		case MENU:
		{
			Menu();
			break;
		}
		case GAME_OVER:
		{
			Status = MENU;
			break;
		}
		}
	}
}

void Game::Menu()
{
	Text MenuWelcome ("Witaj w space shooterze!", font, 65);
	MenuWelcome.setPosition(40, 40);
	const int HowMany = 3;
	Text MenuChoose[HowMany];
	String MenuChooses[] = { "Graj !", "Instrukcja !", "Wyjdz !" };
	for (int i = 0; i < HowMany; i++)
	{
		MenuChoose[i].setFont(font);
		MenuChoose[i].setCharacterSize(45);
		MenuChoose[i].setString(MenuChooses[i]);
		MenuChoose[i].setPosition(40, 150 + (i * 100));
	}
	Ball.launch();
	while (Status == MENU)
	{
		ifstream plik;
		plik.open("Data/Score.txt", ios::app);
		string line;
		int x = 0;
		while (getline(plik, line))
		{
			BestScore = line;
		}
		plik.close();
		Text ScoreText("Najlepszy wynik: ", font, 30);
		Text ScoreStat(BestScore, font, 30);
		ScoreText.setPosition(Vector2f(10, 680));
		ScoreStat.setPosition(Vector2f(240, 680));
		Vector2f MousePosition(Mouse::getPosition(window));
		for (int i = 0; i < HowMany; i++)
		{
			if (MenuChoose[i].getGlobalBounds().contains(MousePosition))
			{
				MenuChoose[i].setColor(Color::Green);
			}
			else MenuChoose[i].setColor(Color::White);
		}
		window.clear();
		window.draw(MenuWelcome);
		window.draw(ScoreText);
		window.draw(ScoreStat);
		for (int i = 0; i < HowMany; i++)
		{
			window.draw(MenuChoose[i]);
		}
		window.draw(MenuBall);
		window.display();
		while (window.pollEvent(WhereToGo))
		{
			if (WhereToGo.type == Event::Closed || WhereToGo.type == Event::KeyPressed && WhereToGo.key.code == Keyboard::Escape)
			{
				Status = END;
			}
			else if (MenuChoose[2].getGlobalBounds().contains(MousePosition) && WhereToGo.type == Event::MouseButtonReleased && WhereToGo.key.code == Mouse::Left)
			{
				Status = END;
			}
			else if (MenuChoose[0].getGlobalBounds().contains(MousePosition) && WhereToGo.type == Event::MouseButtonReleased && WhereToGo.key.code == Mouse::Left)
			{
				Status = GAME;
				StartTheGame();
				Ball.launch();
			}
			else if (MenuChoose[1].getGlobalBounds().contains(MousePosition) && WhereToGo.type == Event::MouseButtonReleased && WhereToGo.key.code == Mouse::Left)
			{
				Instruction();
			}
		}
	}
}

void Game::StartTheGame()
{
	Scripts *NewSession;
	NewSession = new Scripts(window, Status);
	delete NewSession;
}

void Game::JumpingBall()
{
	float x = 3.9;
	float y = 3.9;
	float posx = rand() % 710 + 301;
	float posy = rand() % 505 + 200;
	texture.loadFromFile("Data/asteroid.png");
	MenuBall.setTexture(texture);
	MenuBall.setPosition(posx, posy);
	MenuBall.setScale(Vector2f(0.5, 0.5));
	do
	{
		if (posx > 1014 || posx < 300)
		{
			x = (-x);
		}
		if (posy > 710 || posy < 200)
		{
			y = (-y);
		}
		sleep(milliseconds(17));
		MenuBall.move(Vector2f(x, y));
		posx += x;
		posy += y;
	} while (Status == MENU);
}

void Game::Instruction()
{
	bool Return = false;
	while (Return == false)
	{
		Vector2f MousePosx(Mouse::getPosition(window));
		window.clear();
		Event InstrEnd;
		Text InstrTitle("Instrukcja!", font, 100);
		InstrTitle.setPosition(1024 / 2 - InstrTitle.getGlobalBounds().width / 2, 20);
		Text InstructionText("Strzelaj do asteroidow,", font, 30);
		Text InstructionText1("ruch lewo/prawo - strzalki,", font, 30);
		Text InstructionText3("strzal - spacja.", font, 30);
		Text InstructionText2("Powrot", font, 35);
		InstructionText.setPosition(1024 / 2 - InstructionText.getGlobalBounds().width / 2, 200);
		InstructionText2.setPosition(1024 / 2 - InstructionText2.getGlobalBounds().width / 2, 630);
		InstructionText1.setPosition(1024 / 2 - InstructionText1.getGlobalBounds().width / 2, 250);
		InstructionText3.setPosition(1024 / 2 - InstructionText3.getGlobalBounds().width / 2, 300);
		if (InstructionText2.getGlobalBounds().contains(MousePosx))
		{
			InstructionText2.setColor(Color::Green);
		}
		else InstructionText2.setColor(Color::White);
		window.draw(InstrTitle);
		window.draw(InstructionText);
		window.draw(InstructionText2);
		window.draw(InstructionText3);
		window.draw(InstructionText1);
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