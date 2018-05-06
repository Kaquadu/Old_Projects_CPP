#include <SFML/Graphics.hpp>
#include <cmath>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include "Textures.h"
#include <sstream>
#include <fstream>

using namespace std;
using namespace sf;

RenderWindow window(VideoMode(800, 600), "Rzut Ukosny");

string to_string (float d)
{
    std::ostringstream ss;
    ss << d;
    return ss.str();
}
class Pilka
{
public:

    Pilka()
    {
        pilkaTex.setFillColor(Color::Magenta);
        pilkaTex.setRadius(10);
    };
    ~Pilka(){};
    CircleShape pilkaTex;
    float mass = 1;
    float grav = 9.81;
    float alpha = 0;
    float beginVel = 0;
    float vx;
    float vy;
    float currPredkosc;
    float currWzn;
    float currDroga;
    float currWys;
    float currOdl;
};

class Engine
{
public:
    Engine();
    ~Engine();
    void Update();
    void DrawWindow();
    void LaunchSim();
    void StartSim();
    void SetStartPositions();
    void SetVelocity();
    void SetAlpha();
    void SetMass();
    void SetGrav();
    void SetFileName();
    void Restart();
    string Typing();
    string Typing2();

    Pilka nowaPilka;
    Textures textures;
    Event event;
    View camera;
    bool restart = false;
    bool wykresy = false;
};



int main()
{
    Engine nowaSymulacja;
    nowaSymulacja.LaunchSim();
    return 0;
}

Engine::Engine()
{
    camera.setSize(800, 600);
    camera.setCenter(400, 300);
}

Engine::~Engine()
{
}

void Engine::DrawWindow()
{
    window.clear();

    window.setView(camera);

    window.draw(textures.backgroundSprite);

    window.draw(textures.setAlphaText);
    window.draw(textures.setGravText);
    window.draw(textures.setMassText);
    window.draw(textures.setVelText);
    window.draw(textures.setFileNameText);
    window.draw(textures.saveText);
    window.draw(textures.loadText);

    window.draw(textures.fileNameText);
    window.draw(textures.alphaText);
    window.draw(textures.gravText);
    window.draw(textures.velText);
    window.draw(textures.massText);

    if (wykresy == true)
    {
        window.draw(textures.wykrOdlegloscText);
        window.draw(textures.wykrPredkoscText);
        window.draw(textures.wykrPredkoscWznText);
        window.draw(textures.wykrPrzemieszczenieText);
        window.draw(textures.wykrWysokoscText);
    }

    window.draw(textures.startSimText);
    window.draw(textures.wykresyText);

    window.draw(textures.hitLine);
    window.draw(textures.arrowSprite);
    window.draw(nowaPilka.pilkaTex);

    window.display();
}

void Engine::LaunchSim()
{
    SetStartPositions();
    while (window.isOpen())
    {
        Vector2f MousePos(Mouse::getPosition(window));

        if (textures.setAlphaText.getGlobalBounds().contains(MousePos))
            textures.setAlphaText.setColor(Color::Red);
        else
            textures.setAlphaText.setColor(Color::White);
        if (textures.setGravText.getGlobalBounds().contains(MousePos))
            textures.setGravText.setColor(Color::Red);
        else
            textures.setGravText.setColor(Color::White);
        if (textures.setMassText.getGlobalBounds().contains(MousePos))
            textures.setMassText.setColor(Color::Red);
        else
            textures.setMassText.setColor(Color::White);
        if (textures.setVelText.getGlobalBounds().contains(MousePos))
            textures.setVelText.setColor(Color::Red);
        else
            textures.setVelText.setColor(Color::White);
        if (textures.setFileNameText.getGlobalBounds().contains(MousePos))
            textures.setFileNameText.setColor(Color::Red);
        else
            textures.setFileNameText.setColor(Color::White);
        if (textures.saveText.getGlobalBounds().contains(MousePos))
            textures.saveText.setColor(Color::Red);
        else
            textures.saveText.setColor(Color::White);
        if (textures.loadText.getGlobalBounds().contains(MousePos))
            textures.loadText.setColor(Color::Red);
        else
            textures.loadText.setColor(Color::White);

        while(window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (wykresy == false && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && textures.setVelText.getGlobalBounds().contains(MousePos))
                SetVelocity();
            if (wykresy == false && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && textures.setAlphaText.getGlobalBounds().contains(MousePos))
                SetAlpha();
            if (wykresy == false && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && textures.setGravText.getGlobalBounds().contains(MousePos))
                SetGrav();
            if (wykresy == false && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && textures.setMassText.getGlobalBounds().contains(MousePos))
                SetMass();
            if (wykresy == false && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && textures.setFileNameText.getGlobalBounds().contains(MousePos))
                SetFileName();
            if (wykresy == false && restart == false && event.type == Event::KeyReleased && event.key.code == Keyboard::BackSpace)
                StartSim();
            if (wykresy == false && restart == true && event.type == Event::KeyReleased && event.key.code == Keyboard::End)
                Restart();
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::W)
            {
                if (wykresy == false)
                {
                    wykresy = true;
                    textures.wykresyText.setString("W - schowaj wykresy");
                }
                else
                {
                    wykresy = false;
                    textures.wykresyText.setString("W - pokaz wykresy");
                }
            }
            if (wykresy == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num1)
                system("start wgnuplot -p -e \"plot 'Graphs/Predkosc.txt' w lines  \"");
            if (wykresy == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num2)
                system("start wgnuplot -p -e \"plot 'Graphs/PredkoscWznoszenia.txt' w lines  \"");
            if (wykresy == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num3)
                system("start wgnuplot -p -e \"plot 'Graphs/Przemieszczenie.txt' w lines  \"");
            if (wykresy == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num4)
                system("start wgnuplot -p -e \"plot 'Graphs/Wysokosc.txt' w lines  \"");
            if (wykresy == true && event.type == Event::KeyReleased && event.key.code == Keyboard::Num5)
                system("start wgnuplot -p -e \"plot 'Graphs/Odleglosc.txt' w lines  \"");
            if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && textures.saveText.getGlobalBounds().contains(MousePos))
				{
					string tmp = textures.fileNameText.getString();
					ofstream plik(tmp.c_str());
					if (plik.is_open())
					{
						plik << nowaPilka.mass << endl;
						plik << nowaPilka.alpha << endl;
						plik << nowaPilka.grav << endl;
						plik << nowaPilka.beginVel << endl;
						plik.close();
					}
				}
            if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && textures.loadText.getGlobalBounds().contains(MousePos))
				{
					string tmp = textures.fileNameText.getString();
					ifstream plik(tmp.c_str());
					string line;
					int i = 0;
					while (!plik.eof())
					{
						getline(plik, line);
						switch (i)
						{
						case 0:
							nowaPilka.mass = atof(line.c_str()); break;
						case 1:
							nowaPilka.alpha = atof(line.c_str()); break;
						case 2:
							nowaPilka.grav = atof(line.c_str()); break;
						case 3:
							nowaPilka.beginVel = atof(line.c_str()); break;
						}
						i++;
					}
					textures.massText.setString(to_string(nowaPilka.mass) + " [kg]");
                    textures.gravText.setString(to_string(nowaPilka.grav) + " [m/s^2]");
                    textures.alphaText.setString(to_string(nowaPilka.alpha) + " [deg]");
                    textures.velText.setString(to_string(nowaPilka.beginVel) + " [m/s]");
                    textures.arrowSprite.setRotation(-nowaPilka.alpha);
				}
        }
        DrawWindow();
    }
}

void Engine::SetStartPositions()
{
    textures.setMassText.setPosition(10,10);
    textures.setAlphaText.setPosition(10, 30);
    textures.setGravText.setPosition(10, 50);
    textures.setVelText.setPosition(10, 70);
    textures.setFileNameText.setPosition(10, 90);

    textures.massText.setString(to_string(nowaPilka.mass) + " [kg]");
    textures.gravText.setString(to_string(nowaPilka.grav) + " [m/s^2]");
    textures.alphaText.setString(to_string(nowaPilka.alpha) + " [deg]");
    textures.velText.setString(to_string(nowaPilka.beginVel) + " [m/s]");
    textures.massText.setCharacterSize(15);
    textures.massText.setFont(textures.font);
    textures.massText.setPosition(170, 10);
    textures.gravText.setCharacterSize(15);
    textures.gravText.setFont(textures.font);
    textures.gravText.setPosition(170, 50);
    textures.alphaText.setCharacterSize(15);
    textures.alphaText.setFont(textures.font);
    textures.alphaText.setPosition(170, 30);
    textures.velText.setCharacterSize(15);
    textures.velText.setFont(textures.font);
    textures.velText.setPosition(170, 70);
    textures.fileNameText.setCharacterSize(15);
    textures.fileNameText.setFont(textures.font);
    textures.fileNameText.setPosition(170, 90);
    textures.fileNameText.setString("przyklad.txt");
    textures.saveText.setPosition(10, 130);
    textures.loadText.setPosition(50, 130);

    textures.hitLine.setPosition(0, 560);

    nowaPilka.pilkaTex.setOrigin(10, 10);
    nowaPilka.pilkaTex.setPosition(10, 550);
    textures.arrowSprite.setOrigin(0, 1150);
    textures.arrowSprite.setPosition(10, 550);
    textures.arrowSprite.setScale(0.02, 0.02);

    textures.startSimText.setPosition(camera.getCenter().x, camera.getCenter().y - 280);
    textures.wykresyText.setPosition(camera.getCenter().x, camera.getCenter().y - 250);

    textures.wykrPredkoscText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 30);
    textures.wykrPredkoscWznText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 50);
    textures.wykrPrzemieszczenieText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 70);
    textures.wykrWysokoscText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 90);
    textures.wykrOdlegloscText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 110);
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
	textures.alphaText.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		textures.alphaText.setString(textures.alphaText.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = textures.alphaText.getString();
	if (atof(tmp1.c_str()) > 90)
	{
		textures.alphaText.setString("90");
	}
	if (atof(tmp1.c_str()) < 0)
	{
		textures.alphaText.setString("0");
	}
	tmp1 = textures.alphaText.getString();
	nowaPilka.alpha = atof(tmp1.c_str());
	if (textures.alphaText.getString().getSize() == 0)
	{
		textures.alphaText.setString(to_string(nowaPilka.alpha) + " [deg]");
	}
	else
	{
		textures.alphaText.setString(textures.alphaText.getString() + " [deg]");
	}
	textures.arrowSprite.setRotation((-1)*nowaPilka.alpha);
}

void Engine::SetVelocity()
{
    textures.velText.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		textures.velText.setString(textures.velText.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = textures.velText.getString();
	if (atof(tmp1.c_str()) < 0)
	{
		textures.velText.setString("0.0001");
	}
	tmp1 = textures.velText.getString();
	nowaPilka.beginVel = atof(tmp1.c_str());
	if(textures.velText.getString().getSize() == 0)
	{
		textures.velText.setString(to_string(nowaPilka.beginVel) + " [m/s]");
	}
	else
	{
		textures.velText.setString(textures.velText.getString() + " [m/s]");
	}
}

void Engine::SetGrav()
{
    textures.gravText.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		textures.gravText.setString(textures.gravText.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = textures.gravText.getString();
	if (atof(tmp1.c_str()) < 0)
	{
		textures.gravText.setString("0");
	}
	tmp1 = textures.gravText.getString();
	nowaPilka.grav = atof(tmp1.c_str());
	if (textures.gravText.getString().getSize() == 0)
	{
		textures.gravText.setString(to_string(nowaPilka.grav) + " [m/s^2]");
	}
	else
	{
		textures.gravText.setString(textures.gravText.getString() + " [m/s^2]");
	}
}

void Engine::SetMass()
{
    textures.massText.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing();
		textures.massText.setString(textures.massText.getString() + tmp);
		DrawWindow();
	}
	string tmp1 = textures.massText.getString();
	if (atof(tmp1.c_str()) < 0)
	{
		textures.massText.setString("0.0001");
	}
	tmp1 = textures.massText.getString();
	nowaPilka.mass = atof(tmp1.c_str());
	if (textures.massText.getString().getSize() == 0)
	{
		textures.massText.setString(to_string(nowaPilka.mass) + " [kg]");
	}
	else
	{
		textures.massText.setString(textures.massText.getString() + " [kg]");
	}
}

void Engine::SetFileName()
{
    string tmp = textures.fileNameText.getString();
    textures.fileNameText.setString("");
	while (!Keyboard::isKeyPressed(Keyboard::Return))
	{
		string tmp;
		tmp = Typing2();
		textures.fileNameText.setString(textures.fileNameText.getString() + tmp);
		DrawWindow();
	}
	if (textures.fileNameText.getString().getSize() == 0)
    {
        textures.fileNameText.setString(tmp);
    }
}


void Engine::StartSim()
{
    restart = true;
    textures.startSimText.setString("End - restart symulacji");

    nowaPilka.currDroga = 0;
    nowaPilka.currOdl = 0;
    nowaPilka.currPredkosc = 0;
    nowaPilka.currWys = 0;
    nowaPilka.currWzn = 0;

    nowaPilka.vx = nowaPilka.beginVel * cos(nowaPilka.alpha / 180 * M_PI);
    nowaPilka.vy = nowaPilka.beginVel * sin(nowaPilka.alpha / 180 * M_PI);

    Clock deltaTime;
    Clock risingTime;
    Time clock1;
    Time clock2;

    fstream predkosc;
    fstream predkoscwzn;
    fstream przemieszczenie;
    fstream wysokosc;
    fstream odleglosc;

    predkosc.open("Graphs/Predkosc.txt", ios::trunc | ios::out | ios::in);
    predkoscwzn.open("Graphs/PredkoscWznoszenia.txt", ios::trunc | ios::out | ios::in);
    przemieszczenie.open("Graphs/Przemieszczenie.txt", ios::trunc | ios::out | ios::in);
    wysokosc.open("Graphs/Wysokosc.txt", ios::trunc | ios::out | ios::in);
    odleglosc.open("Graphs/Odleglosc.txt", ios::trunc | ios::out | ios::in);

    do
    {
        sleep(milliseconds(15));
        clock1 = deltaTime.getElapsedTime();
        clock2 = risingTime.getElapsedTime();
        nowaPilka.vy -= nowaPilka.grav * clock1.asSeconds();
        nowaPilka.pilkaTex.move(nowaPilka.vx, -nowaPilka.vy);
        if (nowaPilka.pilkaTex.getPosition().x >= 400)
        {
            camera.move(nowaPilka.vx, 0);
            textures.hitLine.move(nowaPilka.vx, 0);
        }
        nowaPilka.currPredkosc = sqrt(nowaPilka.vx*nowaPilka.vx + nowaPilka.vy*nowaPilka.vy);
        cout << nowaPilka.currPredkosc << endl;
        predkosc << clock2.asSeconds() << " " << nowaPilka.currPredkosc << endl;
        nowaPilka.currDroga += nowaPilka.currPredkosc * clock1.asSeconds();
        przemieszczenie << clock2.asSeconds() << " " << nowaPilka.currDroga << endl;
        nowaPilka.currWzn = nowaPilka.vy;
        predkoscwzn << clock2.asSeconds() << " " << nowaPilka.currWzn << endl;
        nowaPilka.currWys += nowaPilka.currWzn * clock1.asSeconds();
        wysokosc << clock2.asSeconds() << " " << nowaPilka.currWys << endl;
        nowaPilka.currOdl += nowaPilka.vx * clock1.asSeconds();
        odleglosc << clock2.asSeconds() << " " << nowaPilka.currOdl << endl;
        Update();
        DrawWindow();
        deltaTime.restart();
    } while (nowaPilka.pilkaTex.getPosition().y <= 550);
    DrawWindow();

    predkosc.close();
    predkoscwzn.close();
    przemieszczenie.close();
    wysokosc.close();
    odleglosc.close();
}

void Engine::Restart()
{
    nowaPilka.pilkaTex.setPosition(10, 550);
    camera.setCenter(400, 300);
    textures.hitLine.setPosition(0, 560);
    Update();
    textures.startSimText.setString("Backspace - rozpocznij symulacje");
    restart = false;
}

void Engine::Update()
{
    textures.startSimText.setPosition(camera.getCenter().x, camera.getCenter().y - 280);
    textures.wykresyText.setPosition(camera.getCenter().x, camera.getCenter().y - 250);
    textures.wykrPredkoscText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 30);
    textures.wykrPredkoscWznText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 50);
    textures.wykrPrzemieszczenieText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 70);
    textures.wykrWysokoscText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 90);
    textures.wykrOdlegloscText.setPosition(textures.wykresyText.getPosition().x, textures.wykresyText.getPosition().y + 110);
}
