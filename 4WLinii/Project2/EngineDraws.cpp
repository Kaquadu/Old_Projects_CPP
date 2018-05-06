#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <Windows.h>
#include <time.h>

void Engine::SetCirclesValues()
{
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			circle[j][i].setRadius(25);
			circle[j][i].setPosition(233 + (i * 85), 165 + (j * 70));
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