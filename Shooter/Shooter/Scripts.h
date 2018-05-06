#include <SFML/Graphics.hpp>
#include <time.h>
#include <conio.h>
#include <fstream>
#include "Objects.h"

using namespace std;
using namespace sf;

enum WhereAreWe { MENU, GAME, GAME_OVER, END };

class Scripts
{
public:
	Scripts(RenderWindow &window, WhereAreWe &Status);
	~Scripts();
	void RunScripts(RenderWindow &window, WhereAreWe &Status);
	void CreateBalls();
	void MoveBalls();
	void CreateShots();
	void MoveShots();
	void MovePlayerRight(RenderWindow &window);
	void MovePlayerLeft(RenderWindow &window);
	void DrawWindow(RenderWindow &window);
	void Checking_If_Lost();
	void ClearingBuforBall();
	void ClearingBuforBall(int i);
	void ClearingBuforShots();
	void ClearingBuforShots(int i, int k);
	void GameOverBoard(RenderWindow &window, WhereAreWe &Status);
	Player NewPlayer;
	Ball *NewBall[10];
	Shot *NewShot[10];
	Thread BallsCreating;
	Thread BallsMoving;
	Thread ShotsCreating;
	Thread ShotsMoving;
	Event Moving;
	Font font;
	Texture background;
	RectangleShape BackGround;
	int HowManyBalls = 0;
	int HowManyShots = 0;
	int CreatingTime = 2500;
	int Score = 0;
	int Hits = 0;
	bool CheckLost = false;
};

Scripts::Scripts(RenderWindow &window, WhereAreWe &Status)
	: BallsCreating(&Scripts::CreateBalls, this)
	, BallsMoving(&Scripts::MoveBalls, this)
	, ShotsCreating(&Scripts::CreateShots, this)
	, ShotsMoving(&Scripts::MoveShots, this)
{
	background.loadFromFile("Data/back.jpg");
	BackGround.setTexture(&background);
	BackGround.setPosition(Vector2f(0, 0));
	BackGround.setSize(Vector2f(1024, 720));
	font.loadFromFile("Data/font.ttf");
	RunScripts(window, Status);
}

Scripts::~Scripts()
{
}

void Scripts::RunScripts(RenderWindow &window, WhereAreWe &Status)
{
	BallsCreating.launch();
	BallsMoving.launch();
	ShotsCreating.launch();
	ShotsMoving.launch();
	while (Status != GAME_OVER)
	{
		if (CheckLost == true)
		{
			Status = GAME_OVER;
		}
		DrawWindow(window);
		while (window.pollEvent(Moving))
		{
			if (Moving.type == Event::KeyPressed && Moving.key.code == Keyboard::Left)
			{
				MovePlayerLeft(window);
			}
			else if (Moving.type == Event::KeyPressed && Moving.key.code == Keyboard::Right)
			{
				MovePlayerRight(window);
			}
			else if (Moving.type == Event::KeyPressed && Moving.key.code == Keyboard::Escape)
			{
				Status = GAME_OVER;
				CheckLost = true;
			}
		}
	}
	GameOverBoard(window, Status);
}

void Scripts::CreateBalls()
{
	do
	{
		try
		{
			if ((CreatingTime - 500) <= 500)
			{
				throw 0;
			}
			if ((Score > 0) && ((Score % 10) == 0))
			{
				CreatingTime -= 500;
			}
		}
		catch (int) { CreatingTime = 850; }
		catch (...) {};
		sleep(milliseconds(CreatingTime));
		if (HowManyBalls < 10)
		{
			NewBall[HowManyBalls] = new Ball;
			HowManyBalls++;
		}
	} while (CheckLost == false);
}

void Scripts::MoveBalls()
{
	sleep(milliseconds(2500));
	do
	{
		sleep(milliseconds(17));
		for (int i = 0; i < HowManyBalls; i++)
		{
			NewBall[i]->posx += NewBall[i]->movx;
			NewBall[i]->posy += NewBall[i]->movy;
			NewBall[i]->JumpBall.move(Vector2f(NewBall[i]->movx, NewBall[i]->movy));
			if (NewBall[i]->posx > 1014 || NewBall[i]->posx < 15)
			{
				NewBall[i]->movx = (-NewBall[i]->movx);
			}
			else if (NewBall[i]->posy > 705 || NewBall[i]->posy < 15)
			{
				NewBall[i]->movy = (-NewBall[i]->movy);
			}
			else if (NewPlayer.SpaceShip.getGlobalBounds().contains(NewBall[i]->JumpBall.getPosition()))
			{
				ClearingBuforBall(i);
				Hits++;
			}
			Checking_If_Lost();
		}
	} while (CheckLost == false);
}

void Scripts::ClearingBuforBall()
{
	for (int i = 0; i < HowManyBalls-1; i++)
	{
		if (HowManyBalls != 1)
		{
			NewBall[i] = NewBall[i + 1];
		}
		else
		{
			delete NewBall[i];
		}
	}
	HowManyBalls--;
}

void Scripts::ClearingBuforBall(int k)
{
	for (int i = k; i < HowManyBalls-1; i++)
	{
		if (HowManyBalls != 1)
		{
			NewBall[i] = NewBall[i + 1];
		}
		else
		{
			delete NewBall[i];
		}
	}
	HowManyBalls--;
}

void Scripts::CreateShots()
{
	do
	{
		if (Moving.type == Event::KeyPressed && Moving.key.code == Keyboard::Space)
		{
			if (HowManyShots < 10)
			{
				Vector2f Position = NewPlayer.SpaceShip.getPosition();
				NewShot[HowManyShots] = new Shot(Position);
				HowManyShots++;
			}
		}
	} while (CheckLost == false);
}

void Scripts::MoveShots()
{
	do
	{
		sleep(milliseconds(17));
		for (int i = 0; i < HowManyShots; i++)
		{
			NewShot[i]->posy -= 5;
			NewShot[i]->Bullet.move(Vector2f(0, -5));
			if (NewShot[i]->posy < 10)
			{
				ClearingBuforShots();
			}
			for (int k = 0; k < HowManyBalls; k++)
			{
				if (NewBall[k]->JumpBall.getGlobalBounds().contains(NewShot[i]->Bullet.getPosition()))
				{
					ClearingBuforShots(i, k);
					Score++;
				}
			}
		}
	} while (CheckLost == false);
}

void Scripts::ClearingBuforShots()
{
	for (int i = 0; i < HowManyShots-1; i++)
	{
		if (HowManyShots != 1)
		{
			NewShot[i] = NewShot[i + 1];
		}
		else
		{
			delete NewShot[i];
		}
	}
	HowManyShots--;
}

void Scripts::ClearingBuforShots(int i, int k)
{
	ClearingBuforBall(k);
	for (int j = i; j < HowManyShots-1; j++)
	{
		if (HowManyShots != 1)
		{
			NewShot[j] = NewShot[j + 1];
		}
		else
		{
			delete NewShot[j];
		}
	}
	HowManyShots--;
}

void Scripts::MovePlayerLeft(RenderWindow &window)
{
	try
	{
		if (NewPlayer.x <= 35)
		{
			throw 0;
		}
		else
		{
			DrawWindow(window);
			NewPlayer.SpaceShip.move(Vector2f(-15, 0));
			NewPlayer.x -= 15;
			DrawWindow(window);
			sleep(milliseconds(16));
		}
	}
	catch (int) {};
}

void Scripts::MovePlayerRight(RenderWindow &window)
{
	try
	{
		if (NewPlayer.x >= (1024-35))
		{
			throw 0;
		}
		else
		{
			DrawWindow(window);
			NewPlayer.SpaceShip.move(Vector2f(15, 0));
			NewPlayer.x += 15;
			DrawWindow(window);
			sleep(milliseconds(16));
		}
	}
	catch (int) {};
}

void Scripts::DrawWindow(RenderWindow &window)
{
	window.clear();
	int ShotsLeft = 10 - HowManyShots;
	Text ScoreBoard("Score: ", font, 25);
	Text ScoreScore(to_string(Score), font, 25);
	ScoreScore.setPosition(Vector2f(120, 5));
	ScoreBoard.setPosition(Vector2f(5, 5));
	Text HitsBoard("Hits: ", font, 25);
	Text HitsScore(to_string(Hits), font, 25);
	HitsScore.setPosition(Vector2f(300, 5));
	HitsBoard.setPosition(Vector2f(235, 5));
	Text ShotsBoard("Shots: ", font, 25);
	Text ShotsScore(to_string(ShotsLeft), font, 25);
	ShotsScore.setPosition(Vector2f(420, 5));
	ShotsBoard.setPosition(Vector2f(335, 5));
	window.draw(BackGround);
	window.draw(ScoreBoard);
	window.draw(ScoreScore);
	window.draw(HitsBoard);
	window.draw(HitsScore);
	window.draw(ShotsBoard);
	window.draw(ShotsScore);
	window.draw(NewPlayer.SpaceShip);
	for (int k = 0; k < HowManyBalls; k++)
	{
		window.draw(NewBall[k]->JumpBall);
	}
	for (int j = 0; j < HowManyShots; j++)
	{
		window.draw(NewShot[j]->Bullet);
	}
	window.display();
}

void Scripts::Checking_If_Lost()
{
	if (Hits == 3)
	{
		CheckLost = true;
	}
}

void Scripts::GameOverBoard(RenderWindow &window, WhereAreWe &Status)
{
	Event ReturnToMenu;
	string BestScore;
	int BestScoreInt;
	string StrScore;
	ifstream plik;
	plik.open("Data/Score.txt", ios::app);
	string line;
	int x = 0;
	while (getline(plik, line))
	{
		BestScore = line;
	}
	plik.close();
	BestScoreInt = stoi(BestScore);
	StrScore = to_string(Score);
	if (BestScoreInt < Score)
	{
		ofstream plik("Data/Score.txt", ios::out | ios::trunc);
		plik << StrScore;
		plik.close();
	}
	sleep(milliseconds(200));
	while (Status != MENU)
	{
		Vector2f MousePosx(Mouse::getPosition(window));
		Text Score1("Wynik:", font, 40);
		Text Score2(StrScore, font, 40);
		Text Score3("Powrot", font, 40);
		Score1.setPosition(1024 / 2 - Score1.getGlobalBounds().width / 2, 200);
		Score2.setPosition(1024 / 2 - Score2.getGlobalBounds().width / 2, 250);
		Score3.setPosition(1024 / 2 - Score3.getGlobalBounds().width / 2, 600);
		if (Score3.getGlobalBounds().contains(MousePosx))
		{
			Score3.setColor(Color::Green);
		}
		else Score3.setColor(Color::White);
		window.clear();
		window.draw(Score1);
		window.draw(Score2);
		window.draw(Score3);
		window.display();
		while (window.pollEvent(ReturnToMenu))
		{
			if (Event::KeyPressed && ReturnToMenu.key.code == Keyboard::Escape)
			{
				Status = MENU;
			}
			else if (Score3.getGlobalBounds().contains(MousePosx) && ReturnToMenu.type == Event::MouseButtonReleased && ReturnToMenu.key.code == Mouse::Left)
			{
				Status = MENU;
			}
		}
	}
}