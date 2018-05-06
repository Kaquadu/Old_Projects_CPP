#include "stdafx.h"
#include <cmath>
#include <fstream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Pendulum
{
public:
	Pendulum(Texture &penTex);
	~Pendulum() {};
	void Simulation(Time &clock, Time &clock2);
	void OpenFiles();
	void CloseFiles();
	Sprite pendulumSprite;
	float mass = 1;
	float grav = 9.81f;
	float length = 1;
	float alpha = 0;
	float force;
	float acc;
	float speed;
	float rotacc;
	float rotspeed;

	fstream sila;
	fstream przysp;
	fstream predkosc;
	fstream omega;
	fstream przyspkatowe;
	fstream kat;
};

Pendulum::Pendulum(Texture &penTex)
{
	pendulumSprite.setTexture(penTex);
	pendulumSprite.setPosition(250, 10);
	pendulumSprite.setOrigin(56, 0);
	pendulumSprite.setScale(0.5, 0.5);
}

void Pendulum::OpenFiles()
{
	sila.open("Graphs/SilaNapedzajaca.txt", ios::trunc | ios::out | ios::in);
	przysp.open("Graphs/PrzyspieszenieLiniowe.txt", ios::trunc | ios::out | ios::in);
	predkosc.open("Graphs/PredkoscLiniowa.txt", ios::trunc | ios::out | ios::in);
	omega.open("Graphs/PredkoscKatowa.txt", ios::trunc | ios::out | ios::in);
	przyspkatowe.open("Graphs/PrzyspieszenieKatowe.txt", ios::trunc | ios::out | ios::in);
	kat.open("Graphs/Kat.txt", ios::trunc | ios::out | ios::in);
}

void Pendulum::CloseFiles()
{
	sila.close();
	przysp.close();
	predkosc.close();
	omega.close();
	przyspkatowe.close();
	kat.close();
}

void Pendulum::Simulation(Time &clock, Time &clock2)
{
	force = mass * grav * sin(alpha / 180 * 3.141592);
	sila << clock2.asSeconds() << " " << force << endl;
	acc = force / mass;
	przysp << clock2.asSeconds() << " " << acc << endl;
	rotacc = acc / length;
	przyspkatowe << clock2.asSeconds() << " " << rotacc << endl;
	rotspeed += rotacc * (float)clock.asSeconds();
	omega << clock2.asSeconds() << " " << rotspeed << endl;
	speed += acc * (float)clock.asSeconds();
	predkosc << clock2.asSeconds() << " " << speed << endl;
	alpha -= (rotspeed * (float)clock.asSeconds()) * 180 / 3.141592;
	kat << clock2.asSeconds() << " " << alpha << endl;
	pendulumSprite.setRotation(alpha);
}


struct Textures
{
	Textures();
	~Textures() {};
	Texture pendulumTexture;
};

Textures::Textures()
{
	pendulumTexture.loadFromFile("Data/pendulum.png");
}


struct Texts
{
	Texts() { font.loadFromFile("Data/orange_juice.ttf"); };
	~Texts() {};
	Font font;

	Text setLength;
	Text length;
	Text setAlpha;
	Text alpha;
	Text setMass;
	Text mass;
	Text setGrav;
	Text grav;

	Text save;
	Text load;
	Text setFileName;
	Text fileName;

	Text startSim;
	Text graphs;

	Text degGraph;
	Text rotvelGraph;
	Text rotaccGraph;
	Text velGraph;
	Text accGraph;
	Text forceGraph;
};