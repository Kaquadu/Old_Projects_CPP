#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"
#include "Pen.h"

using namespace sf;
using namespace std;

RenderWindow window(VideoMode(1024, 720), "Wahadlo Matematyczne");

class Engine
{
public:
	Engine();
	~Engine();
	void StartProgram();
	void SetValues();
	void DrawWindow();
	void SetAlpha();
	void SetMass();
	void SetGrav();
	void SetLength();
	void SetFileName();
	void LaunchSimulation();
	string Typing();
	string Typing2();
	Textures textures;
	Pendulum pendulum;
	RectangleShape background;
	Texts texts;
	Event event;
	bool showGraphs = false;
};

Engine::Engine()
	: pendulum(textures.pendulumTexture)
{
}

Engine::~Engine()
{

}

void Engine::StartProgram()
{
	SetValues();
	do
	{
		Vector2f MousePos(Mouse::getPosition(window));

		if (texts.setAlpha.getGlobalBounds().contains(MousePos))
			texts.setAlpha.setColor(Color::Color(255, 0, 0));
		else
			texts.setAlpha.setColor(Color::Color(255, 128, 0));
		if (texts.setGrav.getGlobalBounds().contains(MousePos))
			texts.setGrav.setColor(Color::Color(255, 0, 0));
		else
			texts.setGrav.setColor(Color::Color(255, 128, 0));
		if (texts.setMass.getGlobalBounds().contains(MousePos))
			texts.setMass.setColor(Color::Color(255, 0, 0));
		else
			texts.setMass.setColor(Color::Color(255, 128, 0));
		if (texts.setLength.getGlobalBounds().contains(MousePos))
			texts.setLength.setColor(Color::Color(255, 0, 0));
		else
			texts.setLength.setColor(Color::Color(255, 128, 0));
		if (texts.startSim.getGlobalBounds().contains(MousePos))
			texts.startSim.setColor(Color::Color(255, 0, 0));
		else
			texts.startSim.setColor(Color::Color(255, 128, 0));
		if (texts.graphs.getGlobalBounds().contains(MousePos))
			texts.graphs.setColor(Color::Color(255, 0, 0));
		else
			texts.graphs.setColor(Color::Color(255, 128, 0));
		if (texts.setFileName.getGlobalBounds().contains(MousePos))
			texts.setFileName.setColor(Color::Color(255, 0, 0));
		else
			texts.setFileName.setColor(Color::Color(255, 128, 0));
		if (texts.load.getGlobalBounds().contains(MousePos))
			texts.load.setColor(Color::Color(255, 0, 0));
		else
			texts.load.setColor(Color::Color(255, 128, 0));
		if (texts.save.getGlobalBounds().contains(MousePos))
			texts.save.setColor(Color::Color(255, 0, 0));
		else
			texts.save.setColor(Color::Color(255, 128, 0));

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				window.close();
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.setAlpha.getGlobalBounds().contains(MousePos))
			{
				SetAlpha();
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.setGrav.getGlobalBounds().contains(MousePos))
			{
				SetGrav();
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.setMass.getGlobalBounds().contains(MousePos))
			{
				SetMass();
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.setLength.getGlobalBounds().contains(MousePos))
			{
				SetLength();
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.startSim.getGlobalBounds().contains(MousePos))
			{
				LaunchSimulation();
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.setFileName.getGlobalBounds().contains(MousePos))
			{
				SetFileName();
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.save.getGlobalBounds().contains(MousePos))
			{
				string tmp = texts.fileName.getString();
				ofstream plik(tmp.c_str());
				if (plik.is_open())
				{
					plik << pendulum.length << endl;
					plik << pendulum.mass << endl;
					plik << pendulum.alpha << endl;
					plik << pendulum.grav << endl;
					plik.close();
				}
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.load.getGlobalBounds().contains(MousePos))
			{
				string tmp = texts.fileName.getString();
				ifstream plik(tmp.c_str());
				string line;
				int i = 0;
				while (!plik.eof())
				{
					getline(plik, line);
					switch (i)
					{
					case 0:
						pendulum.length = stof(line); break;
					case 1:
						pendulum.mass = stof(line); break;
					case 2:
						pendulum.alpha = stof(line); break;
					case 3:
						pendulum.grav = stof(line); break;
					}
					i++;
				}
				texts.alpha.setString(to_string(pendulum.alpha) + " [deg]");
				texts.mass.setString(to_string(pendulum.mass) + " [kg]");
				texts.length.setString(to_string(pendulum.length) + " [m]");
				texts.grav.setString(to_string(pendulum.grav) + " [m/s^2]");
				pendulum.pendulumSprite.setRotation(pendulum.alpha);
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && texts.graphs.getGlobalBounds().contains(MousePos))
			{
				if (showGraphs == false)
					showGraphs = true;
				else
					showGraphs = false;
			}
			if (showGraphs == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num1)
			{
				system("start wgnuplot -p -e \"plot 'Graphs/SilaNapedzajaca.txt' w lines  \"");
			}
			if (showGraphs == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num2)
			{
				system("start wgnuplot -p -e \"plot 'Graphs/Kat.txt' w lines  \"");
			}
			if (showGraphs == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num3)
			{
				system("start wgnuplot -p -e \"plot 'Graphs/PrzyspieszenieLiniowe.txt' w lines  \"");
			}
			if (showGraphs == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num4)
			{
				system("start wgnuplot -p -e \"plot 'Graphs/PredkoscLiniowa.txt' w lines  \"");
			}
			if (showGraphs == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num5)
			{
				system("start wgnuplot -p -e \"plot 'Graphs/PredkoscKatowa.txt' w lines  \"");
			}
			if (showGraphs == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num6)
			{
				system("start wgnuplot -p -e \"plot 'Graphs/PrzyspieszenieKatowe.txt' w lines  \"");
			}
		}
		DrawWindow();
	} while (window.isOpen());
}

void Engine::SetValues()
{
	background.setFillColor(Color::Color(128, 128, 128));
	background.setPosition(0, 0);
	background.setSize(Vector2f(1024, 720));

	texts.setAlpha.setFont(texts.font);
	texts.setAlpha.setString("Zmien kat nachylenia");
	texts.setAlpha.setColor(Color::Color(255, 128, 0));
	texts.setAlpha.setCharacterSize(30);
	texts.setAlpha.setPosition(550, 20);

	texts.setGrav.setFont(texts.font);
	texts.setGrav.setString("Zmien grawitacje");
	texts.setGrav.setColor(Color::Color(255, 128, 0));
	texts.setGrav.setCharacterSize(30);
	texts.setGrav.setPosition(550, 60);

	texts.setMass.setFont(texts.font);
	texts.setMass.setString("Zmian mase");
	texts.setMass.setColor(Color::Color(255, 128, 0));
	texts.setMass.setCharacterSize(30);
	texts.setMass.setPosition(550, 100);

	texts.setLength.setFont(texts.font);
	texts.setLength.setString("Zmien dlugosc");
	texts.setLength.setColor(Color::Color(255, 128, 0));
	texts.setLength.setCharacterSize(30);
	texts.setLength.setPosition(550, 140);

	texts.setFileName.setFont(texts.font);
	texts.setFileName.setString("Plik zapisu/odczytu");
	texts.setFileName.setColor(Color::Color(255, 128, 0));
	texts.setFileName.setCharacterSize(30);
	texts.setFileName.setPosition(550, 180);

	texts.fileName.setFont(texts.font);
	texts.fileName.setString("przyklad.txt");
	texts.fileName.setColor(Color::Color(255, 128, 0));
	texts.fileName.setCharacterSize(15);
	texts.fileName.setPosition(850, 190);
	
	texts.alpha.setFont(texts.font);
	texts.alpha.setString(to_string(pendulum.alpha) + " [deg]");
	texts.alpha.setColor(Color::Color(255, 128, 0));
	texts.alpha.setCharacterSize(15);
	texts.alpha.setPosition(850, 30);

	texts.grav.setFont(texts.font);
	texts.grav.setString(to_string(pendulum.grav) + " [m/s^2]");
	texts.grav.setColor(Color::Color(255, 128, 0));
	texts.grav.setCharacterSize(15);
	texts.grav.setPosition(850, 70);

	texts.mass.setFont(texts.font);
	texts.mass.setString(to_string(pendulum.mass) + " [kg]");
	texts.mass.setColor(Color::Color(255, 128, 0));
	texts.mass.setCharacterSize(15);
	texts.mass.setPosition(850, 110);

	texts.length.setFont(texts.font);
	texts.length.setString(to_string(pendulum.length) + " [m]");
	texts.length.setColor(Color::Color(255, 128, 0));
	texts.length.setCharacterSize(15);
	texts.length.setPosition(850, 150);

	texts.startSim.setFont(texts.font);
	texts.startSim.setString("Start symulacji");
	texts.startSim.setColor(Color::Color(255, 128, 0));
	texts.startSim.setCharacterSize(30);
	texts.startSim.setPosition(20, 450);

	texts.graphs.setFont(texts.font);
	texts.graphs.setString("Pokaz wykresy");
	texts.graphs.setColor(Color::Color(255, 128, 0));
	texts.graphs.setCharacterSize(30);
	texts.graphs.setPosition(250, 450);

	texts.save.setFont(texts.font);
	texts.save.setString("Zapisz");
	texts.save.setColor(Color::Color(255, 128, 0));
	texts.save.setCharacterSize(30);
	texts.save.setPosition(475, 450);

	texts.load.setFont(texts.font);
	texts.load.setString("Wczytaj");
	texts.load.setColor(Color::Color(255, 128, 0));
	texts.load.setCharacterSize(30);
	texts.load.setPosition(595, 450);

	texts.forceGraph.setFont(texts.font);
	texts.forceGraph.setString("1. Sila / czas");
	texts.forceGraph.setColor(Color::Color(255, 128, 0));
	texts.forceGraph.setCharacterSize(15);
	texts.forceGraph.setPosition(250, 490);

	texts.degGraph.setFont(texts.font);
	texts.degGraph.setString("2. Kat / czas");
	texts.degGraph.setColor(Color::Color(255, 128, 0));
	texts.degGraph.setCharacterSize(15);
	texts.degGraph.setPosition(250, 510);

	texts.accGraph.setFont(texts.font);
	texts.accGraph.setString("3. Przysp. / czas");
	texts.accGraph.setColor(Color::Color(255, 128, 0));
	texts.accGraph.setCharacterSize(15);
	texts.accGraph.setPosition(250, 530);

	texts.velGraph.setFont(texts.font);
	texts.velGraph.setString("4. Predk. / czas");
	texts.velGraph.setColor(Color::Color(255, 128, 0));
	texts.velGraph.setCharacterSize(15);
	texts.velGraph.setPosition(250, 550);

	texts.rotvelGraph.setFont(texts.font);
	texts.rotvelGraph.setString("5. Predk. kat. / czas");
	texts.rotvelGraph.setColor(Color::Color(255, 128, 0));
	texts.rotvelGraph.setCharacterSize(15);
	texts.rotvelGraph.setPosition(250, 570);

	texts.rotaccGraph.setFont(texts.font);
	texts.rotaccGraph.setString("6. Przysp. kat. / czas");
	texts.rotaccGraph.setColor(Color::Color(255, 128, 0));
	texts.rotaccGraph.setCharacterSize(15);
	texts.rotaccGraph.setPosition(250, 590);
}

void Engine::DrawWindow()
{
	window.clear();

	window.draw(background);

	window.draw(texts.setAlpha);
	window.draw(texts.setMass);
	window.draw(texts.setLength);
	window.draw(texts.setGrav);

	window.draw(texts.alpha);
	window.draw(texts.grav);
	window.draw(texts.mass);
	window.draw(texts.length);

	window.draw(texts.startSim);
	window.draw(texts.graphs);
	window.draw(texts.save);
	window.draw(texts.load);

	window.draw(texts.fileName);
	window.draw(texts.setFileName);

	if (showGraphs == true)
	{
		window.draw(texts.rotaccGraph);
		window.draw(texts.rotvelGraph);
		window.draw(texts.accGraph);
		window.draw(texts.degGraph);
		window.draw(texts.forceGraph);
		window.draw(texts.velGraph);
	}

	window.draw(pendulum.pendulumSprite);

	window.display();
}

string Engine::Typing()
{
	string Return;
	while (window.pollEvent(event))
	{
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num1)
			Return = "1";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num2)
			Return = "2";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num3)
			Return = "3";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num4)
			Return = "4";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num5)
			Return = "5";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num6)
			Return = "6";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num7)
			Return = "7";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num8)
			Return = "8";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num9)
			Return = "9";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num0)
			Return = "0";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Period)
			Return = ".";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Dash)
			Return = "-";
	}
return Return;
}

string Engine::Typing2()
{
	string Return;
	while (window.pollEvent(event))
	{
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num1)
			Return = "1";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num2)
			Return = "2";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num3)
			Return = "3";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num4)
			Return = "4";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num5)
			Return = "5";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num6)
			Return = "6";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num7)
			Return = "7";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num8)
			Return = "8";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num9)
			Return = "9";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Num0)
			Return = "0";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Period)
			Return = ".";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::W)
			Return = "w";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Q)
			Return = "q";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::E)
			Return = "e";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::R)
			Return = "r";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::T)
			Return = "t";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Y)
			Return = "y";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::U)
			Return = "u";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::I)
			Return = "i";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::O)
			Return = "o";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::P)
			Return = "p";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::A)
			Return = "a";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::S)
			Return = "s";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::D)
			Return = "d";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::F)
			Return = "f";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::G)
			Return = "g";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::H)
			Return = "h";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::J)
			Return = "j";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::K)
			Return = "k";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::L)
			Return = "l";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::Z)
			Return = "z";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::X)
			Return = "x";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::C)
			Return = "c";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::V)
			Return = "v";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::B)
			Return = "b";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::N)
			Return = "n";
		if (event.type == Event::KeyReleased && event.key.code == Keyboard::M)
			Return = "m";
	}
	return Return;
}

void Engine::SetAlpha()
{
	texts.alpha.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		texts.alpha.setString(texts.alpha.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = texts.alpha.getString();
	if (stof(tmp1) > 90)
	{
		texts.alpha.setString("90");
	}
	if (stof(tmp1) < -90)
	{
		texts.alpha.setString("-90");
	}
	tmp1 = texts.alpha.getString();
	pendulum.alpha = stof(tmp1);
	if (texts.alpha.getString().getSize() == 0)
	{
		texts.alpha.setString(to_string(pendulum.alpha) + " [deg]");
	}
	else
	{
		texts.alpha.setString(texts.alpha.getString() + " [deg]");
	}
	pendulum.pendulumSprite.setRotation(pendulum.alpha);
}

void Engine::SetGrav()
{
	texts.grav.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		texts.grav.setString(texts.grav.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = texts.grav.getString();
	if (stof(tmp1) < 0)
	{
		texts.grav.setString("0");
	}
	tmp1 = texts.grav.getString();
	pendulum.grav = stof(tmp1);
	if (texts.grav.getString().getSize() == 0)
	{
		texts.grav.setString(to_string(pendulum.grav) + " [m/s^2]");
	}
	else
	{
		texts.grav.setString(texts.grav.getString() + " [m/s^2]");
	}
}

void Engine::SetMass()
{
	texts.mass.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		texts.mass.setString(texts.mass.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = texts.mass.getString();
	if (stof(tmp1) < 0)
	{
		texts.mass.setString("0.0001");
	}
	tmp1 = texts.mass.getString();
	pendulum.mass = stof(tmp1);
	if (texts.mass.getString().getSize() == 0)
	{
		texts.mass.setString(to_string(pendulum.mass) + " [kg]");
	}
	else
	{
		texts.mass.setString(texts.mass.getString() + " [kg]");
	}
}

void Engine::SetLength()
{
	texts.length.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		texts.length.setString(texts.length.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = texts.length.getString();
	if (stof(tmp1) < 0)
	{
		texts.length.setString("0.0001");
	}
	tmp1 = texts.length.getString();
	pendulum.length = stof(tmp1);
	if (texts.length.getString().getSize() == 0)
	{
		texts.length.setString(to_string(pendulum.length) + " [m]");
	}
	else
	{
		texts.length.setString(texts.length.getString() + " [m]");
	}
}

void Engine::SetFileName()
{
	string tmp2 = texts.fileName.getString();
	texts.fileName.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing2();
		texts.fileName.setString(texts.fileName.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = texts.fileName.getString();
	if (texts.fileName.getString().getSize() == 0)
	{
		texts.fileName.setString(tmp2);
	}
	else
	{
		texts.fileName.setString(texts.fileName.getString() + ".txt");
	}
}

void Engine::LaunchSimulation()
{
	pendulum.force = 0;
	pendulum.acc = 0;
	pendulum.speed = 0;
	pendulum.rotacc = 0;
	pendulum.rotspeed = 0;
	Clock deltaTime;
	Clock risingTime;
	Time clock;
	Time clock2;
	pendulum.OpenFiles();
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		sleep(milliseconds(15));
		clock = deltaTime.getElapsedTime();
		clock2 = risingTime.getElapsedTime();
		pendulum.Simulation(clock, clock2);
		Vector2f MousePos(Mouse::getPosition(window));
		DrawWindow();
		deltaTime.restart();
	}
	pendulum.CloseFiles();
}