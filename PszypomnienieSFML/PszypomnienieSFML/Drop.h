#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "SFML\Graphics.hpp"

using namespace std;
using namespace sf;

enum PickUpType {MisslePick, DecreaseReload, Kit};

class PickUp
{
public:
	~PickUp() {};
	PickUpType Type;
	Sprite PickUpSprite;
	FloatRect Collidor;
	float Angle = 90;
	void Rotate();
	static PickUp *createNewPickUp(float x, float y, Texture &PickUpTex, PickUpType Type);
};

class MisslePickUp :public PickUp
{
public:
	MisslePickUp(float x, float y, Texture &PickUpTex)
	{
		Type = MisslePick;
		PickUpSprite.setTexture(PickUpTex);
		PickUpSprite.setPosition(x, y);
		PickUpSprite.setScale(0.2, 0.2);
		Collidor = PickUpSprite.getGlobalBounds();
	};
	~MisslePickUp() {};
};

class DecreaseReloadPickUp :public PickUp
{
public:
	DecreaseReloadPickUp(float x, float y, Texture &PickUpTex)
	{
		PickUpSprite.setTexture(PickUpTex);
		PickUpSprite.setPosition(x, y);
	};
	~DecreaseReloadPickUp() {};
};

class KitPickUp :public PickUp
{
public:
	KitPickUp(float x, float y, Texture &PickUpTex)
	{
		Type = Kit;
		PickUpSprite.setTexture(PickUpTex);
		PickUpSprite.setPosition(x, y);
		PickUpSprite.setScale(0.1, 0.1);
		Collidor = PickUpSprite.getGlobalBounds();
	};
	~KitPickUp() {};
};

PickUp *PickUp::createNewPickUp(float x, float y, Texture &PickUpTex, PickUpType Type)
{
	switch (Type)
	{
		case MisslePick:
		{
			return new MisslePickUp(x, y, PickUpTex);
			break;
		}
		case DecreaseReload:
		{
			return new DecreaseReloadPickUp(x, y, PickUpTex);
			break;
		}
		case Kit:
		{
			return new KitPickUp(x, y, PickUpTex);
			break;
		}
	}
}

void PickUp::Rotate()
{
	Angle += 0.15;
	PickUpSprite.setRotation(Angle);
	Collidor = PickUpSprite.getGlobalBounds();
}