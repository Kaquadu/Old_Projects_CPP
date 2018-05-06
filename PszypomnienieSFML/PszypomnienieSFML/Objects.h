#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "SFML\Graphics.hpp"
#include <cmath>
#include <fstream>

enum ObjectType {PLANET, ASTEROID, MISSLE, ALIEN, ALIENMISSLE};

using namespace std;
using namespace sf;

class GameTextures
{
public:
	GameTextures()
	{
		EarthTex.loadFromFile("Data/Textures/Earth.png");
		MissleTex.loadFromFile("Data/Textures/Missle.png");
		MissleBoomTex.loadFromFile("Data/Textures/Explosion.png");
		AlienTex.loadFromFile("Data/Textures/Alien.png");
		RocketPickupTex.loadFromFile("Data/Textures/RocketSprite.png");
		KitTex.loadFromFile("Data/Textures/Kit.png");
		ReloadTex.setFillColor(Color::Yellow);
	};
	~GameTextures() {};
	Texture EarthTex;
	Texture MissleTex;
	Texture MissleBoomTex;
	Texture RocketPickupTex;
	Texture PlanetTex;
	Texture AsteroidTex;
	Texture AlienTex;
	Texture KitTex;
	RectangleShape ReloadTex;
};

class GameObject
{
public:
	~GameObject() {};
	ObjectType Type;
	Texture ObjectTexture;
	Sprite ObjectSprite;
	FloatRect Collidor;
	long float PosX;
	long float PosY;
	float Speed;
	float deltaTime;
	float ObAngle = 90;
	int HP;
	int MaxHP;
	int Damage;
	float ReloadTime = 1.8;
	float Reload = 0.16;
	void Move(GameObject &NewPlayer);
	bool BoomRockets(Texture &ObjectTex);
	static GameObject *createNewObject(ObjectType type, long float x, long float y, float angle, int Damage, Texture &ObjectTex);
};

class Player :public GameObject
{
public:
	Player(RenderWindow &window, float ResX, float ResY);
	~Player();
	void setAngle(float alpha);
	float getAngle();
	float angle;
	float Acceleration = 0.05f;
	float VelX = 0;
	float VelY = 0;
	float MaxVel = 6;
	float ReloadTime = 0.7f;
	float Reload = 0.0f;
	int Level = 1;
	int CurrentExp;
	int ExpNeed;
	int Damage;
	int MaxHP = 90 + (Level * Level) * 10;
	float RegenTime = 5;
	float CurrentRegenTime = 5;
	bool CanRegen();
	void Heal();
	void Move();
	void LevelUp();
};

class Planet :public GameObject
{
public:
	Planet(long float x, long float y, Texture &ObjectTex);
	~Planet() {};
};

class Asteroid :public GameObject
{
public:
	Asteroid(long float x, long float y, Texture &ObjectTex) {};
	~Asteroid() {};
};

class Missle :public GameObject
{
public:
	Missle(long float x, long float y, float angle, int Dmg, Texture &ObjectTex);
	~Missle() {};
};

class Alien :public GameObject
{
public:
	Alien(long float x, long float y, Texture &ObjectTex, int Level, int Range);
	~Alien() {};
};

class AlienMissle :public GameObject
{
public:
	AlienMissle(long float x, long float y, float angle, int Dmg, Texture &ObjectTex);
	~AlienMissle() {};
};

Player::Player(RenderWindow &window, float ResX, float ResY)
{
	Font font;
	fstream file;
	file.open("Data/PlayerInfo/Stats.txt");
	string line;
	int temph = 0;
	while (!file.eof())
	{
		getline(file, line);
		switch (temph)
		{
		case 0:
		{
			Level = stof(line);
			//cout << line << endl;
			break;
		}
		case 1:
		{
			CurrentExp = stof(line);
			//cout << line;
			break;
		}
		}
		temph++;
	}
	file.close();
	//cout << "Level: " << Level << endl;
	//cout << "CurrentExp: " << CurrentExp << endl;
	ExpNeed = 75 * (Level * Level);
	ObjectTexture.loadFromFile("Data/Textures/Player.png");
	ObjectSprite.setTexture(ObjectTexture);
	ObjectSprite.setOrigin(81, 108);
	ObjectSprite.setScale(0.5, 0.5);
	Collidor = ObjectSprite.getGlobalBounds();
	MaxHP = 90 + (Level * Level) * 10;
	Damage = MaxHP / 10;
	HP = MaxHP;
	PosX = ResX / 2;
	PosY = ResY / 2;
	cout << ExpNeed << endl;
}

Player::~Player()
{
}

void Player::setAngle(float alpha)
{
	angle = alpha;
}

float Player::getAngle()
{
	return angle;
}

bool Player::CanRegen()
{
	if (CurrentRegenTime <= 0)
		return true;
	else
		return false;
}

void Player::Heal()
{
	CurrentRegenTime = RegenTime;
	HP += (MaxHP / 10);
	if (HP > MaxHP)
		HP = MaxHP;
}

void Player::Move()
{
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		if (VelY > -4)
			VelY -= Acceleration;
		else
			VelY = -4;
		//NewPlayer.ObjectSprite.move(0, -6);
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		//NewPlayer.ObjectSprite.move(0, 6);
		if (VelY < 4)
			VelY += Acceleration;
		else
			VelY = 4;
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		//NewPlayer.ObjectSprite.move(6, 0);
		if (VelX < 4)
			VelX += Acceleration;
		else
			VelX = 4;
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		if (VelX > -4)
			VelX -= Acceleration;
		else
			VelX = -4;
		//NewPlayer.ObjectSprite.move(-6, 0);
	}
	/*if (Keyboard::isKeyPressed(Keyboard::B) && (NewPlayer.VelX < 0.5 || NewPlayer.VelX > -0.5) && (NewPlayer.VelY < 0.5 || NewPlayer.VelY > -0.5))
	{
	NewPlayer.VelX = 0;
	NewPlayer.VelY = 0;
	}*/
	PosX += VelX;
	PosY += VelY;
	ObjectSprite.move(VelX, VelY);
	Collidor = ObjectSprite.getGlobalBounds();
}

void Player::LevelUp()
{
	/*cout << "Level: " << Level << endl;
	cout << "CurrentExp: " << CurrentExp << endl;
	cout << "ExpNeed: " << ExpNeed << endl;*/
	Level++;
	CurrentExp = CurrentExp - ExpNeed;
	MaxHP = 90 + (Level * Level) * 10;
	HP = MaxHP;
	ExpNeed = 75 * (Level * Level);
	/*cout << "Level: " << Level << endl;
	cout << "CurrentExp: " << CurrentExp << endl;
	cout << "ExpNeed: " << ExpNeed << endl;*/
}

void GameObject::Move(GameObject &NewPlayer)
{
	if ((Type == MISSLE || Type == ALIENMISSLE) && deltaTime < 2.4)
	{
		deltaTime += 0.016;
		ObjectSprite.move(Speed*sin(ObAngle / 180 * 3.14), -Speed*cos(ObAngle / 180 * 3.14));
		Collidor = ObjectSprite.getGlobalBounds();
	}
	else if (Type == PLANET)
	{
		ObAngle += 0.05;
		ObjectSprite.setRotation(ObAngle);
	}
	else if (Type == ALIEN)
	{
		float PI = 3.14159265;
		float dx = ObjectSprite.getPosition().x - NewPlayer.ObjectSprite.getPosition().x;
		float dy = ObjectSprite.getPosition().y - NewPlayer.ObjectSprite.getPosition().y;
		float alpha = atan2f(dy, dx) * 180 / PI + 90;
		if (alpha >= 0)
		{
			ObjectSprite.setRotation(alpha);
			ObAngle = alpha;
		}
		else
		{
			ObjectSprite.setRotation(alpha + 360);
			ObAngle = alpha;
		}

		ObjectSprite.move(-Speed*sin(ObAngle / 180 * 3.14), Speed*cos(ObAngle / 180 * 3.14));
		Collidor = ObjectSprite.getGlobalBounds();
	}
}

bool GameObject::BoomRockets(Texture &ObjectTex)
{
		if (deltaTime >= 2.4 && deltaTime <= 2.55)
		{
			deltaTime += 0.016;
			ObjectSprite.setScale(0.2, 0.2);
			ObjectSprite.setOrigin(256, 256);
			ObjectSprite.setTexture(ObjectTex);
		}
		if (deltaTime >= 2.55 && deltaTime <= 3.5)
			deltaTime += 0.016;
		if (deltaTime >= 3.5)
			return true;
		else
			return false;
}

GameObject *GameObject::createNewObject(ObjectType type, long float x, long float y, float angle, int Dmg, Texture &ObjectTex)
{
	switch (type)
	{
	case MISSLE:
	{
		return new Missle(x, y, angle, Dmg, ObjectTex);
		break;
	}
	case ALIENMISSLE:
	{
		return new AlienMissle(x, y, angle, Dmg, ObjectTex);
		break;
	}
	case PLANET:
	{
		return new Planet(x, y, ObjectTex);
		break;
	}
	case ASTEROID:
	{
		return new Asteroid(x, y, ObjectTex);
		break;
	}
	case ALIEN:
	{
		return new Alien(x, y, ObjectTex, angle, Dmg);
		break;
	}
	default:
		break;
	}
}

Planet::Planet(long float x, long float y, Texture &ObjectTex)
{
	Type = PLANET;
	ObjectSprite.setTexture(ObjectTex);
	ObjectSprite.setOrigin(512, 512);
	ObjectSprite.setScale(0.5, 0.5);
	ObjectSprite.setPosition(x, y);
	Collidor = ObjectSprite.getGlobalBounds();
}

Missle::Missle(long float x, long float y, float angle, int Dmg, Texture &ObjectTex)
{
	Type = MISSLE;
	deltaTime = 0;
	ObjectSprite.setTexture(ObjectTex);
	ObjectSprite.setOrigin(277, 1053);
	ObjectSprite.setScale(0.03, 0.03);
	ObjectSprite.setPosition(x, y);
	ObjectSprite.setRotation(angle);
	Damage = Dmg;
	Speed = 6.5;
	HP = 9999;
	if (angle >= 0)
		ObAngle = angle;
	else
		ObAngle = angle + 360;
	Collidor = ObjectSprite.getGlobalBounds();
}

Alien::Alien(long float x, long float y, Texture &ObjectTex, int Level, int Range)
{
	Type = ALIEN;
	MaxHP = (Range / 10);
	HP = MaxHP;
	Speed = 2.5f;
	ObjectSprite.setTexture(ObjectTex);
	ObjectSprite.setOrigin(63, 119);
	ObjectSprite.setScale(0.7, 0.7);
	ObjectSprite.setPosition(x, y);
	Collidor = ObjectSprite.getGlobalBounds();
}

AlienMissle::AlienMissle(long float x, long float y, float angle, int Dmg, Texture &ObjectTex)
{
	Type = ALIENMISSLE;
	deltaTime = 0;
	ObjectSprite.setTexture(ObjectTex);
	ObjectSprite.setOrigin(277, 1053);
	ObjectSprite.setScale(0.03, 0.03);
	ObjectSprite.setPosition(x, y);
	ObjectSprite.setRotation(angle);
	Damage = Dmg;
	Speed = 4.5;
	HP = 9999;
	if (angle >= 0)
		ObAngle = angle;
	else
		ObAngle = angle + 360;
	//cout << ObAngle << endl;
	Collidor = ObjectSprite.getGlobalBounds();
}