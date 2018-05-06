#include <SFML/Graphics.hpp>
#include <time.h>
#include <conio.h>

using namespace std;
using namespace sf;

class Player
{
public:
	Player();
	~Player();
	Texture spaceship;
	RectangleShape SpaceShip;
	float x = 512;
	float y = (720-120);
};

Player::Player()
{
	spaceship.loadFromFile("Data/spaceship.png");
	SpaceShip.setSize(Vector2f(70, 120));
	SpaceShip.setOrigin(Vector2f(35, 1));
	SpaceShip.setPosition(Vector2f(x, y));
	SpaceShip.setTexture(&spaceship);
}

Player::~Player()
{
}

class Shot
{
public:
	Shot(Vector2f Position);
	~Shot();
	RectangleShape Bullet;
	float posx;
	float posy;
};

Shot::Shot(Vector2f Position)
{
	Bullet.setFillColor(Color::White);
	Bullet.setSize(Vector2f(5, 30));
	Bullet.setPosition(Position);
	posx = Position.x;
	posy = Position.y;
}

Shot::~Shot()
{
}

class Ball
{
public:
	Ball();
	~Ball();
	Texture jumpball;
	Sprite JumpBall;
	float posx;
	float posy = 20;
	float movx;
	float movy;
};

Ball::Ball()
{
	posx = rand() % 1024 + 1;
	jumpball.loadFromFile("Data/asteroid.png");
	JumpBall.setPosition(Vector2f(posx, posy));
	JumpBall.setScale(Vector2f(0.5, 0.5));
	movx = rand() % 7 - 3;
	movy = rand() % 7 - 3;
	JumpBall.setTexture(jumpball);
}

Ball::~Ball()
{
}