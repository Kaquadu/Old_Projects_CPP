#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <string>
#include <Windows.h>
#include <time.h>

enum GameStatus { MENU, INSTR, GAME_MP, GAME_SP, GAME_OVER, END, PLAYER_ONE, PLAYER_TWO, STATS };
enum Board {row = 6, col = 7};
using namespace std;
using namespace sf;

class Datas
{
public:
	bool Winner = false;
	bool Round = false;
	bool GoToMenu = false;
	bool MapDraw = false;
	bool Computer = false;
	bool Mode = false;
	bool NotThrowCol[6];
	Datas() {}
	~Datas() {};
	void setNotThrowCol()
	{
		for (int i = 0; i < 7; i++)
		{
			NotThrowCol[i] = false;
		}
	}
};

class Engine
{
public:
	Engine(RenderWindow &win, GameStatus &SessionStatus);
	Engine(bool Mode, RenderWindow &win, GameStatus &SessionStatus);
	~Engine();
	void runEngineMultiplayer(RenderWindow &window, GameStatus &SessionStatus);
	void runEngineSingleplayer(RenderWindow &window, GameStatus &SessionStatus);
	//void PlayerPutsIn(RenderWindow &window);
	void Check_If_Won();
	void PutIn(RenderWindow &window, int column, GameStatus &SessionStatus);
	void DrawCircles(RenderWindow &window);
	void SetCirclesValues();
	void CircleColorSet(RenderWindow &window, GameStatus &SessionStatus, int column);
	void Vertical_Check();
	void Horizontal_Check();
	void VerHor_Check();
	void WhoWon(GameStatus &SessionStatus, RenderWindow &window);
	void Check_If_Draw(RenderWindow &window);
	void Throw(RenderWindow &window, GameStatus &SessionStatus, Event &OurChoose);
	void IfDrawElse(RenderWindow &window, GameStatus &SessionStatus);
	void ComputerMove(RenderWindow &window, GameStatus &SessionStatus);
	void Can_Computer_Win(bool &CanCWin, RenderWindow &window, GameStatus &SessionStatus);
	void Can_Player_Win(bool &CanCWin, RenderWindow &window, GameStatus &SessionStatus);
	void Pretty_Good_Move(RenderWindow &window, GameStatus &SessionStatus);
	int LastFreeCircle(int column);
	RectangleShape board[7];
	Texture boardTexture1;
	Texture boardTexture2;
	CircleShape circle[row][col];
	Font font;
	Datas NewMap;
	int k,p;
	bool HelpColor;
	int iloscwygranych[2];
};

Engine::Engine(RenderWindow &win, GameStatus &SessionStatus)
{
	if (!font.loadFromFile("debug/FFF_Tusj.ttf"))
	{
		MessageBox(NULL, "Font not found!", "ERROR", NULL);
		return;
	}
	if (!boardTexture1.loadFromFile("debug/board.png"))
	{
		MessageBox(NULL, "Board Texture Not Found!", "ERROR", NULL);
		return;
	}
	if (!boardTexture2.loadFromFile("debug/board2.jpg"))
	{
		MessageBox(NULL, "Board Texture Not Found!", "ERROR", NULL);
		return;
	}
	runEngineMultiplayer(win, SessionStatus);
}

Engine::Engine(bool Mode, RenderWindow &win, GameStatus &SessionStatus)
{
	if (!font.loadFromFile("debug/FFF_Tusj.ttf"))
	{
		MessageBox(NULL, "Font not found!", "ERROR", NULL);
		return;
	}
	if (!boardTexture1.loadFromFile("debug/board.png"))
	{
		MessageBox(NULL, "Board Texture Not Found!", "ERROR", NULL);
		return;
	}
	if (!boardTexture2.loadFromFile("debug/board2.jpg"))
	{
		MessageBox(NULL, "Board Texture Not Found!", "ERROR", NULL);
		return;
	}
	NewMap.Mode = Mode;
	runEngineSingleplayer(win, SessionStatus);
}

Engine::~Engine()
{
	NewMap.~Datas();
}

void Engine::runEngineMultiplayer(RenderWindow &window, GameStatus &SessionStatus)
{
	ifstream plik("dane.txt");
	//plik.open("data/dane.txt", std::ios::in | std::ios::out);
		string line;
		int x = 0;
		while (getline(plik, line))
		{
			iloscwygranych[x] = stoi(line);
			x++;
		}
		plik.close();
	SessionStatus = PLAYER_ONE;
	while (SessionStatus != GAME_OVER)
	{
		//cout << "While SessionStatus != GAME_OVER" << endl;
		Text IngamePlayer1("Player One!", font, 33);
		Text IngamePlayer2("Player Two!", font, 33);
		Text IngameTitle("Banzai!", font, 60);
		IngameTitle.setPosition(1024 / 2 - IngameTitle.getGlobalBounds().width / 2, 20);
		IngamePlayer1.setPosition(830, 200);
		IngamePlayer2.setPosition(830, 250);
		for (int i = 0; i < 7; i++)
		{
			board[i].setSize(Vector2f(86, 420));
			board[i].setPosition(211 + (i * 86), 150);
			if (i % 2 == 0)
			{
				board[i].setTexture(&boardTexture1);
			}
			else
			{
				board[i].setTexture(&boardTexture2);
			}
		}
		SetCirclesValues();
		while (NewMap.Winner == false)
		{
			//cout << "While NewMapWinner == False" << endl;
			Event OurChoose;
			IngamePlayer1.setColor(Color::White);
			IngamePlayer2.setColor(Color::White);
			window.clear();
			window.draw(IngameTitle);
			if (SessionStatus == PLAYER_ONE)
			{
				IngamePlayer1.setColor(Color::Red);
			}
			else if (SessionStatus == PLAYER_TWO)
			{
				IngamePlayer2.setColor(Color::Yellow);
			}
			window.draw(IngamePlayer1);
			window.draw(IngamePlayer2);
			for (int i = 0; i < 7; i++)
			{
				window.draw(board[i]);
			}
			DrawCircles(window);
			window.display();
			switch (SessionStatus)
			{
			case PLAYER_ONE:
			{
				//cout << "Case Player_One" << endl;
				while (NewMap.Round == false)
				{
					Throw(window, SessionStatus, OurChoose);
					//Check_If_Draw(window);
					IfDrawElse(window, SessionStatus);
				}
				break;
			}
			case PLAYER_TWO:
			{
				//cout << "Case Player_Two" << endl;
				while (NewMap.Round == true)
				{
					Throw(window, SessionStatus, OurChoose);
					//Check_If_Draw(window);
					IfDrawElse(window, SessionStatus);
				}
				break;
			}
			}
		}
		WhoWon(SessionStatus, window);
	}
}

void Engine::runEngineSingleplayer(RenderWindow &window, GameStatus &SessionStatus)
{
	SessionStatus = PLAYER_ONE;
	while (SessionStatus != GAME_OVER)
	{
		Text IngamePlayer1("Player One!", font, 33);
		Text IngamePlayer2("Computer!", font, 33);
		Text IngameTitle("Banzai!", font, 60);
		IngameTitle.setPosition(1024 / 2 - IngameTitle.getGlobalBounds().width / 2, 20);
		IngamePlayer1.setPosition(830, 200);
		IngamePlayer2.setPosition(830, 250);
		for (int i = 0; i < 7; i++)
		{
			board[i].setSize(Vector2f(86, 420));
			board[i].setPosition(211 + (i * 86), 150);
			if (i % 2 == 0)
			{
				board[i].setTexture(&boardTexture1);
			}
			else
			{
				board[i].setTexture(&boardTexture2);
			}
		}
		SetCirclesValues();
		while (NewMap.Winner == false)
		{
			//cout << "While NewMapWinner == False" << endl;
			Event OurChoose;
			IngamePlayer1.setColor(Color::White);
			IngamePlayer2.setColor(Color::White);
			window.clear();
			window.draw(IngameTitle);
			if (SessionStatus == PLAYER_ONE)
			{
				IngamePlayer1.setColor(Color::Red);
			}
			else if (SessionStatus == PLAYER_TWO)
			{
				IngamePlayer2.setColor(Color::Yellow);
			}
			window.draw(IngamePlayer1);
			window.draw(IngamePlayer2);
			for (int i = 0; i < 7; i++)
			{
				window.draw(board[i]);
			}
			DrawCircles(window);
			window.display();
			switch (SessionStatus)
			{
			case PLAYER_ONE:
			{
				//cout << "Case Player_One" << endl;
				while (NewMap.Round == false)
				{
					Throw(window, SessionStatus, OurChoose);
					IfDrawElse(window, SessionStatus);
				}
				break;
			}
			case PLAYER_TWO:
			{
				//cout << "Case Player_Two" << endl;
				while (NewMap.Round == true)
				{
					ComputerMove(window, SessionStatus);
					IfDrawElse(window, SessionStatus);
				}
				break;
			}
			}
		}
		WhoWon(SessionStatus, window);
	}
}

void Engine::WhoWon(GameStatus &SessionStatus, RenderWindow &window)
{
	while (SessionStatus != GAME_OVER)
	{
		Vector2f MousePos(Mouse::getPosition(window));
		Event OurChoose;
		if (NewMap.MapDraw == false)
		{
			window.clear();
			Text Player1_Win("Player Two (YELLOW) Won!", font, 60);
			Player1_Win.setPosition(1024 / 2 - Player1_Win.getGlobalBounds().width / 2, 20);
			Text Player2_Win("Player One (RED) Won!", font, 60);
			Player2_Win.setPosition(1024 / 2 - Player2_Win.getGlobalBounds().width / 2, 20);
			switch (SessionStatus)
			{
			case PLAYER_ONE:
			{
				window.draw(Player1_Win);
				break;
			}
			case PLAYER_TWO:
			{
				window.draw(Player2_Win);
				break;
			}
			}

			ofstream plik("dane.txt", ios::app);
			//plik.open("data/dane.txt", ios::app);
				plik.clear();
				if (SessionStatus == PLAYER_ONE)
				{
					plik << iloscwygranych[0] + 1;
					plik << iloscwygranych[1];
				}
				else if (SessionStatus == PLAYER_TWO)
				{
					plik << iloscwygranych[0];
					plik << iloscwygranych[1]+1;
				}
				plik.close();
		}
		else
		{
			window.clear();
			Text Draw_Win("It's a draw!", font, 60);
			Draw_Win.setPosition(1024 / 2 - Draw_Win.getGlobalBounds().width / 2, 20);
			window.draw(Draw_Win);
		}
		Text BackToMenu("Back To Menu", font, 35);
		BackToMenu.setPosition(1024 / 2 - BackToMenu.getGlobalBounds().width / 2, 350);
		if (BackToMenu.getGlobalBounds().contains(MousePos))
		{
			BackToMenu.setColor(Color::Red);
		}
		else BackToMenu.setColor(Color::White);
		window.draw(BackToMenu);
		window.display();
		while (window.pollEvent(OurChoose))
		{
			if (Event::KeyPressed && OurChoose.key.code == Keyboard::BackSpace)
			{
				SessionStatus = GAME_OVER;
			}
			else if (BackToMenu.getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
			{
				SessionStatus = GAME_OVER;
			}
		}
	}
}

void Engine::Check_If_Won()
{
	Vertical_Check();
	Horizontal_Check();
	VerHor_Check();
}

void Engine::Check_If_Draw(RenderWindow &window)
{
	p = 0;
	//cout << "Checking if draw?" << endl;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (circle[i][j].getFillColor() == Color::White)
			{
				p++;
			}
		}
	}
	if (p == 0)
	{
		//cout << "DRAW!!!!" << endl;
		NewMap.MapDraw = true;
		NewMap.Winner = true;
	}
}

void Engine::Horizontal_Check()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (circle[i][j].getFillColor() == Color::Yellow && circle[i][j+1].getFillColor() == Color::Yellow && circle[i][j+2].getFillColor() == Color::Yellow && circle[i][j+3].getFillColor() == Color::Yellow)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i][j + 1].setOutlineThickness(4);
					circle[i][j + 2].setOutlineThickness(4);
					circle[i][j + 3].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i][j + 1].setOutlineColor(Color::Green);
					circle[i][j + 2].setOutlineColor(Color::Green);
					circle[i][j + 3].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "Horizontal Won" << endl;
			}
			if (circle[i][j].getFillColor() == Color::Red && circle[i][j+1].getFillColor() == Color::Red && circle[i][j+2].getFillColor() == Color::Red && circle[i][j+3].getFillColor() == Color::Red)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i][j + 1].setOutlineThickness(4);
					circle[i][j + 2].setOutlineThickness(4);
					circle[i][j + 3].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i][j + 1].setOutlineColor(Color::Green);
					circle[i][j + 2].setOutlineColor(Color::Green);
					circle[i][j + 3].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "Horizontal Won" << endl;
			}
		}
	}
}

void Engine::Vertical_Check()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (circle[i][j].getFillColor() == Color::Yellow && circle[i + 1][j].getFillColor() == Color::Yellow && circle[i + 2][j].getFillColor() == Color::Yellow && circle[i + 3][j].getFillColor() == Color::Yellow)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i + 1][j].setOutlineThickness(4);
					circle[i + 2][j].setOutlineThickness(4);
					circle[i + 3][j].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i + 1][j].setOutlineColor(Color::Green);
					circle[i + 2][j].setOutlineColor(Color::Green);
					circle[i + 3][j].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "Vertical Won" << endl;
			}
			if (circle[i][j].getFillColor() == Color::Red && circle[i + 1][j].getFillColor() == Color::Red && circle[i + 2][j].getFillColor() == Color::Red && circle[i + 3][j].getFillColor() == Color::Red)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i + 1][j].setOutlineThickness(4);
					circle[i + 2][j].setOutlineThickness(4);
					circle[i + 3][j].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i + 1][j].setOutlineColor(Color::Green);
					circle[i + 2][j].setOutlineColor(Color::Green);
					circle[i + 3][j].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "Vertical Won" << endl;
			}
		}
	}
}

void Engine::VerHor_Check()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (circle[i][j].getFillColor() == Color::Yellow && circle[i + 1][j + 1].getFillColor() == Color::Yellow && circle[i + 2][j + 2].getFillColor() == Color::Yellow && circle[i + 3][j + 3].getFillColor() == Color::Yellow)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i + 1][j + 1].setOutlineThickness(4);
					circle[i + 2][j + 2].setOutlineThickness(4);
					circle[i + 3][j + 3].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i + 1][j + 1].setOutlineColor(Color::Green);
					circle[i + 2][j + 2].setOutlineColor(Color::Green);
					circle[i + 3][j + 3].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "ChorVer Won" << endl;
			}
			if (circle[i][j].getFillColor() == Color::Red && circle[i + 1][j + 1].getFillColor() == Color::Red && circle[i + 2][j + 2].getFillColor() == Color::Red && circle[i + 3][j + 3].getFillColor() == Color::Red)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i + 1][j + 1].setOutlineThickness(4);
					circle[i + 2][j + 2].setOutlineThickness(4);
					circle[i + 3][j + 3].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i + 1][j + 1].setOutlineColor(Color::Green);
					circle[i + 2][j + 2].setOutlineColor(Color::Green);
					circle[i + 3][j + 3].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "ChorVer Won" << endl;
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 6; j > 2; j--)
		{
			if (circle[i][j].getFillColor() == Color::Yellow && circle[i + 1][j - 1].getFillColor() == Color::Yellow && circle[i + 2][j - 2].getFillColor() == Color::Yellow && circle[i + 3][j - 3].getFillColor() == Color::Yellow)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i + 1][j - 1].setOutlineThickness(4);
					circle[i + 2][j - 2].setOutlineThickness(4);
					circle[i + 3][j - 3].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i + 1][j - 1].setOutlineColor(Color::Green);
					circle[i + 2][j - 2].setOutlineColor(Color::Green);
					circle[i + 3][j - 3].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "ChorVer Won" << endl;
			}
			if (circle[i][j].getFillColor() == Color::Red && circle[i + 1][j - 1].getFillColor() == Color::Red && circle[i + 2][j - 2].getFillColor() == Color::Red && circle[i + 3][j - 3].getFillColor() == Color::Red)
			{
				if (NewMap.Computer == false)
				{
					circle[i][j].setOutlineThickness(4);
					circle[i + 1][j - 1].setOutlineThickness(4);
					circle[i + 2][j - 2].setOutlineThickness(4);
					circle[i + 3][j - 3].setOutlineThickness(4);
					circle[i][j].setOutlineColor(Color::Green);
					circle[i + 1][j - 1].setOutlineColor(Color::Green);
					circle[i + 2][j - 2].setOutlineColor(Color::Green);
					circle[i + 3][j - 3].setOutlineColor(Color::Green);
				}
				NewMap.Winner = true;
				//cout << "ChorVer Won" << endl;
			}
		}
	}
}

void Engine::PutIn(RenderWindow &window, int column, GameStatus &SessionStatus)
{
	//cout << "Put In A Circle" << endl;
	//cout << "Column: " << column << endl;
	switch (SessionStatus)
	{
	case PLAYER_ONE:
		{
			//cout << "PutIn Case One" << endl;
			CircleColorSet(window, SessionStatus, column);
			break;
		}
	case PLAYER_TWO:
		{
			//cout << "PutIn Case Two" << endl;
			CircleColorSet(window, SessionStatus, column);
			break;
		}
	}
}

void Engine::CircleColorSet(RenderWindow &window, GameStatus &SessionStatus, int column)
{
	switch (SessionStatus)
	{
		case PLAYER_ONE:
		{
			//cout << "Circle Color Set PLAYER ONE" << endl;
			try
			{
				//cout << "Probuje Pokolorowac!" << endl;
				circle[LastFreeCircle(column)][column].setFillColor(Color::Red);
				SessionStatus = PLAYER_TWO;
				NewMap.Round = true;
				Check_If_Draw(window);
			}
			catch (int) { /*cout << "Full column!!!" << endl;*/ SessionStatus = PLAYER_ONE; NewMap.Round = false;}
			catch (...) {}
			break;
		}
		case PLAYER_TWO:
		{
			//cout << "Circle Color Set PLAYER TWO" << endl;
			try
			{
				//cout << "Probuje Pokolorowac!" << endl;
				circle[LastFreeCircle(column)][column].setFillColor(Color::Yellow);
				SessionStatus = PLAYER_ONE;
				NewMap.Round = false;
				Check_If_Draw(window);
			}
			catch (int) { /*cout << "Full column!!!"<< endl;*/ SessionStatus = PLAYER_TWO; NewMap.Round = true;}
			catch (...) {}
			break;
		}
	}
}

int Engine::LastFreeCircle(int column)
{
	if (circle[0][column].getFillColor() != Color::White)
	{
		throw 0;
	}
	//cout << "Last Free Circle Function." << endl;
	for (int i = 0; i < row; i++)
	{
		if (circle[i][column].getFillColor() == Color::White)
		{
			//cout << i << " ColorHelp True" << endl;
			HelpColor = true;
		}
		else
		{
			//cout << i << " ColorHelp False" << endl;
			HelpColor = false;
		}
		switch (HelpColor)
		{
			case true:
			{
				if (i == 5)
				{
					k = i;
				}
				break;
			}
			case false:
			{
				k = i - 1;
				break;
			}
		}
		if (HelpColor == false)
		{
			break;
		}
	}
	//cout << "Nasze k: " << k << endl;
	return k;
}

void Engine::SetCirclesValues()
{
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			circle[j][i].setRadius(25);
			circle[j][i].setPosition(231 + (i * 86), 165 + (j * 70));
			circle[j][i].setFillColor(Color::White);
		}
	}
}

void Engine::DrawCircles(RenderWindow &window)
{
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			window.draw(circle[j][i]);
		}
	}
}

void Engine::Throw(RenderWindow &window, GameStatus &SessionStatus, Event &OurChoose)
{
	Vector2f MousePos(Mouse::getPosition(window));
	while (window.pollEvent(OurChoose))
	{
		if (board[0].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
		{
			PutIn(window, 0, SessionStatus);
		}
		else if (board[1].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
		{
			PutIn(window, 1, SessionStatus);
		}
		else if (board[2].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
		{
			PutIn(window, 2, SessionStatus);
		}
		else if (board[3].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
		{
			PutIn(window, 3, SessionStatus);
		}
		else if (board[4].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
		{
			PutIn(window, 4, SessionStatus);
		}
		else if (board[5].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
		{
			PutIn(window, 5, SessionStatus);
		}
		else if (board[6].getGlobalBounds().contains(MousePos) && OurChoose.type == Event::MouseButtonReleased && OurChoose.key.code == Mouse::Left)
		{
			PutIn(window, 6, SessionStatus);
		}
	}
}

void Engine::IfDrawElse(RenderWindow &window, GameStatus &SessionStatus)
{
	if (NewMap.MapDraw == false)
	{
		Check_If_Won();
		if (NewMap.Winner == true)
		{
			Event TestChoose;
			bool TestCheck = false;
			while (TestCheck == false)
			{
				window.clear();
				Text IngamePlayer1("Player One!", font, 33);
				Text IngamePlayer2("Player Two!", font, 33);
				Text IngameTitle("Banzai!", font, 60);
				Text WinnerMessage("Game won! Press here to continiue", font, 33);
				IngameTitle.setPosition(1024 / 2 - IngameTitle.getGlobalBounds().width / 2, 20);
				IngamePlayer1.setPosition(830, 200);
				IngamePlayer2.setPosition(830, 250);
				WinnerMessage.setPosition(1024 / 2 - WinnerMessage.getGlobalBounds().width / 2, 650);
				window.draw(IngameTitle);
				window.draw(IngamePlayer1);
				window.draw(IngamePlayer2);
				for (int i = 0; i < 7; i++)
				{
					window.draw(board[i]);
				}
				DrawCircles(window);
				Vector2f MousePos(Mouse::getPosition(window));
				if (WinnerMessage.getGlobalBounds().contains(MousePos))
				{
					WinnerMessage.setColor(Color::Red);
				}
				else WinnerMessage.setColor(Color::White);
				window.draw(WinnerMessage);
				window.display();
				while (window.pollEvent(TestChoose))
				{
					if (WinnerMessage.getGlobalBounds().contains(MousePos) && TestChoose.type == Event::MouseButtonReleased && TestChoose.key.code == Mouse::Left)
					{
						TestCheck = true;
					}
					else if (TestChoose.type == Event::KeyReleased && TestChoose.key.code == Keyboard::BackSpace)
					{
						TestCheck = true;
					}
				}
			}
		}
	}
	else
	{
		Event TestChoose;
		bool TestCheck = false;
		while (TestCheck == false)
		{
			window.clear();
			Text IngamePlayer1("Player One!", font, 33);
			Text IngamePlayer2("Player Two!", font, 33);
			Text IngameTitle("Banzai!", font, 60);
			Text WinnerMessage("It's a draw! Press here to continiue", font, 33);
			IngameTitle.setPosition(1024 / 2 - IngameTitle.getGlobalBounds().width / 2, 20);
			IngamePlayer1.setPosition(830, 200);
			IngamePlayer2.setPosition(830, 250);
			WinnerMessage.setPosition(1024 / 2 - WinnerMessage.getGlobalBounds().width / 2, 650);
			window.draw(IngameTitle);
			window.draw(IngamePlayer1);
			window.draw(IngamePlayer2);
			for (int i = 0; i < 7; i++)
			{
				window.draw(board[i]);
			}
			DrawCircles(window);
			Vector2f MousePos(Mouse::getPosition(window));
			if (WinnerMessage.getGlobalBounds().contains(MousePos))
			{
				WinnerMessage.setColor(Color::Red);
			}
			else WinnerMessage.setColor(Color::White);
			window.draw(WinnerMessage);
			window.display();
			while (window.pollEvent(TestChoose))
			{
				if (WinnerMessage.getGlobalBounds().contains(MousePos) && TestChoose.type == Event::MouseButtonReleased && TestChoose.key.code == Mouse::Left)
				{
					TestCheck = true;
				}
				else if (TestChoose.type == Event::KeyReleased && TestChoose.key.code == Keyboard::BackSpace)
				{
					TestCheck = true;
				}
			}
		}
	}
}


void Engine::ComputerMove(RenderWindow &window, GameStatus &SessionStatus)
{
	//cout << "Computer Move" << endl;
	NewMap.Computer = true;
	bool CanCWin = false;
	bool CanPWin = false;
	Can_Computer_Win(CanCWin, window, SessionStatus);
	if (CanCWin == false)
	{
		Can_Player_Win(CanPWin, window, SessionStatus);
	}
	if (CanCWin == false && CanPWin == false)
	{
		Pretty_Good_Move(window, SessionStatus);
	}
	NewMap.Computer = false;
}

void Engine::Can_Computer_Win(bool &CanCWin, RenderWindow &window, GameStatus &SessionStatus)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (circle[i][j].getFillColor() == Color::White)
			{
				if (LastFreeCircle(j) == i)
				{
					circle[i][j].setFillColor(Color::Yellow);
					Check_If_Won();
					if (NewMap.Winner == true)
					{
						//cout << "I won!" << endl;
						NewMap.Computer = false;
						circle[i][j].setFillColor(Color::White);
						CanCWin = true;
						NewMap.Winner = false;
						PutIn(window, j, SessionStatus);
						Check_If_Won();
						NewMap.Computer = true;
						break;
					}
					else
					{
						circle[i][j].setFillColor(Color::White);
					}
				}
			}
		}
		if (NewMap.Winner == true)
		{
			break;
		}
	}
}

void Engine::Can_Player_Win(bool &CanPWin, RenderWindow &window, GameStatus &SessionStatus)
{
	//cout << "Can player win?" << endl;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (circle[i][j].getFillColor() == Color::White)
			{
				if (LastFreeCircle(j) == i)
				{
					//cout << "SetRed" << endl;
					circle[i][j].setFillColor(Color::Red);
					Check_If_Won();
					if (NewMap.Winner == true)
					{
						//cout << "True winner" << endl;
						//cout << "I won!" << endl;
						circle[i][j].setFillColor(Color::White);
						CanPWin = true;
						NewMap.Winner = false;
						PutIn(window, j, SessionStatus);
						Check_If_Won();
						break;
					}
					else
					{
						//cout << "Else winner" << endl;
						circle[i][j].setFillColor(Color::White);
					}
				}
			}
		}
		if (CanPWin == true)
		{
			break;
		}
	}
}

void Engine::Pretty_Good_Move(RenderWindow &window, GameStatus &SessionStatus)
{
	if (NewMap.Mode == false)
	{
		//cout << "MODE FALSE" << endl;
		int a = 0;
		int habababa = 0;
		NewMap.setNotThrowCol();
		for (int i = 1; i < 6; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (circle[i][j].getFillColor() == Color::White)
				{
					if (LastFreeCircle(j) == i)
					{
						circle[i][j].setFillColor(Color::Yellow);
						circle[i - 1][j].setFillColor(Color::Red);
						Check_If_Won();
						if (NewMap.Winner == true)
						{
							//cout << "Nie moge wrzucic w " << j << endl;
							circle[i][j].setFillColor(Color::White);
							circle[i - 1][j].setFillColor(Color::White);
							NewMap.Winner = false;
							NewMap.NotThrowCol[j] = true;
							break;
						}
						else
						{
							circle[i][j].setFillColor(Color::White);
							circle[i - 1][j].setFillColor(Color::White);
						}
					}
				}
			}
		}
		//cout << "Wchodze w do" << endl;
		do
		{
			a = rand() % 7;
			habababa++;
		} while (NewMap.NotThrowCol[a] == true || habababa == 7);
		PutIn(window, a, SessionStatus);
	}
	else
	{
		PutIn(window, rand() % 7, SessionStatus);
	}
}