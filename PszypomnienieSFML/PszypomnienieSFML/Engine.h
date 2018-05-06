#include "stdafx.h"
#include "SFML\Graphics.hpp"
#include "Objects.h"
#include "Drop.h"
#include <vector>

enum InGameStatus { MENU, INGAME, END, GAME_OVER };

using namespace std;
using namespace sf;

class Engine
{
	public:
		Engine(RenderWindow &window, float ResX, float ResY);
		~Engine();
		Event PlayerMove;
		Player NewPlayer;
		Thread FixedUpdateT;
		Vector2f Mouse;
		vector <GameObject*> Objects;
		vector <PickUp*> PickUps;
		GameTextures LoadTextures;
			RectangleShape ScreenRes;
			FloatRect Collidor;
			View Camera;
		int ResoX;
		int ResoY;
		int MonsterInt;
			void Update(InGameStatus &Status, Font &font, RenderWindow &window);
			void FixedUpdate();
			void LaunchGame(RenderWindow &window, InGameStatus &Status, float ResX, float ResY, Font font);
			void RotatePlayer();
			void DrawGame(RenderWindow &window, Player &NewPlayer, Font font, float ResX, float ResY);
			bool IsInViewRange(GameObject &Objects);
			bool IsInViewRange(PickUp &Objects);
			void CollideObjects();
			void CreatingMonsters();
			void MonsterAttack();
			void MoveScreenCollidor();
			int PlayerToEarth();
			bool RangeDelete(GameObject &Objects);
			bool RangeDelete(PickUp &PickUps);
};

Engine::Engine(RenderWindow &window, float ResX, float ResY)
	: NewPlayer(window, ResX, ResY)
	, FixedUpdateT(&Engine::FixedUpdate, this)
{;
	ScreenRes.setSize(Vector2f(ResX, ResY));
	Camera.setSize(ResX, ResY);
	Camera.setCenter(ResX / 2, ResY / 2);
	ResoX = ResX;
	ResoY = ResY;
}

Engine::~Engine()
{
}

void Engine::LaunchGame(RenderWindow &window, InGameStatus &Status, float ResX, float ResY, Font font)
{
	fstream file;
	file.open("Data/PlayerInfo/Stats.txt", ios::trunc | ios::in | ios::out);
	NewPlayer.ObjectSprite.setPosition(ResX / 2, ResY / 2);
	Objects.clear();
	Objects.push_back(GameObject::createNewObject(PLANET, 0, 0, 0, 0, LoadTextures.EarthTex));
	FixedUpdateT.launch();
	while (Status != GAME_OVER)
	{
		Update(Status, font, window);
		if (NewPlayer.HP <= 0)
			Status = GAME_OVER;
		file.open("Data/PlayerInfo/Stats.txt", ios::trunc | ios::in | ios::out);
		if (NewPlayer.HP > 0)
		{
			file << to_string(NewPlayer.Level) << endl;
			file << to_string(NewPlayer.CurrentExp);
		}
		else
		{
			file << to_string(1) << endl;
			file << to_string(0);
		}
		file.close();
	}
}

void Engine::DrawGame(RenderWindow &window, Player &NewPlayer, Font font, float ResX, float ResY)
{
	window.clear();
	window.setView(Camera);
	Text PositionX("Position X: " + to_string((int)NewPlayer.PosX), font, 20);
	Text PositionY("Position Y: " + to_string((int)NewPlayer.PosY), font, 20);
	Text CurrHP("Current HP: " + to_string(NewPlayer.HP) + "/" + to_string(NewPlayer.MaxHP), font, 20);
	Text CurrExp("Current XP: " + to_string(NewPlayer.CurrentExp) + "/" + to_string(NewPlayer.ExpNeed), font, 20);
	//Vector2f ReloadSize(NewPlayer.Reload * 110, 10.0f);
	//LoadTextures.ReloadTex.setSize(ReloadSize);
	//LoadTextures.ReloadTex.setPosition(NewPlayer.ObjectSprite.getPosition().x + ResX / 2 - 25, NewPlayer.ObjectSprite.getPosition().y + ResY / 2 - 70);
	CurrHP.setPosition(NewPlayer.ObjectSprite.getPosition().x - ResX / 2 + 25, NewPlayer.ObjectSprite.getPosition().y - ResY / 2 + 10);
	PositionX.setPosition(NewPlayer.ObjectSprite.getPosition().x - ResX / 2 + 25, NewPlayer.ObjectSprite.getPosition().y - ResY / 2 + 40);
	PositionY.setPosition(NewPlayer.ObjectSprite.getPosition().x - ResX / 2 + 25, NewPlayer.ObjectSprite.getPosition().y - ResY / 2 + 70);
	CurrExp.setPosition(NewPlayer.ObjectSprite.getPosition().x - ResX / 2 + 25, NewPlayer.ObjectSprite.getPosition().y - ResY / 2 + 110);
	for (int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i] && IsInViewRange(*Objects[i]))
		{
			window.draw(Objects[i]->ObjectSprite);
		}
	}
	for (int i = 0; i < PickUps.size(); i++)
	{
		if (PickUps[i] && IsInViewRange(*PickUps[i]))
		{
			window.draw(PickUps[i]->PickUpSprite);
		}
	}
	//window.draw(LoadTextures.ReloadTex);
	window.draw(NewPlayer.ObjectSprite);
	window.draw(CurrHP);
	window.draw(CurrExp);
	window.draw(PositionX);
	window.draw(PositionY);
	window.display();
}

void Engine::Update(InGameStatus &Status, Font &font, RenderWindow &window)
{
	DrawGame(window, NewPlayer, font, ResoX, ResoY);
	while (window.pollEvent(PlayerMove))
	{
		if (PlayerMove.type == Event::KeyPressed && PlayerMove.key.code == Keyboard::Escape)
		{
			Status = GAME_OVER;
		}
	}
	if (NewPlayer.CurrentExp >= NewPlayer.ExpNeed)
		NewPlayer.LevelUp();
}

void Engine::FixedUpdate()
{
	while (NewPlayer.HP > 0)
	{
		MonsterInt = 0;
		for (int i = 0; i < Objects.size(); i++)
		{
			if (Objects[i] && Objects[i]->Type == ALIEN)
				MonsterInt++;
		}
		RotatePlayer();
		if (Mouse::isButtonPressed(Mouse::Left) && NewPlayer.Reload <= 0)
		{
			Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, NewPlayer.angle, NewPlayer.Damage, LoadTextures.MissleTex));
			Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, NewPlayer.angle + 15, NewPlayer.Damage, LoadTextures.MissleTex));
			Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, NewPlayer.angle - 15, NewPlayer.Damage, LoadTextures.MissleTex));
			NewPlayer.Reload = NewPlayer.ReloadTime;
		}
		CreatingMonsters();
		MonsterAttack();
		if (NewPlayer.Reload >= 0.0f)
			NewPlayer.Reload -= 0.016;
		if (NewPlayer.CurrentRegenTime > 0)
			NewPlayer.CurrentRegenTime -= 0.016;
		if (NewPlayer.CanRegen())
			NewPlayer.Heal();
		NewPlayer.Move();
		MoveScreenCollidor();
		for (int i = 0; i < Objects.size(); i++)
		{
			if (Objects[i] && Objects[i]->Reload >= 0.0f)
				Objects[i]->Reload -= 0.016;
			Objects[i]->Move(NewPlayer);
		}
		for (int i = 0; i < Objects.size(); i++)
		{
			if (Objects[i] && (Objects[i]->Type == MISSLE || Objects[i]->Type == ALIENMISSLE) && Objects[i]->BoomRockets(LoadTextures.MissleBoomTex))
				Objects.erase(Objects.begin() + i);
		}
		for (int i = 0; i < Objects.size(); i++)
		{
			if (Objects[i] && (Objects[i]->Type == MISSLE || Objects[i]->Type == ALIENMISSLE || Objects[i]->Type == ALIEN))
			{
				if (RangeDelete(*Objects[i]))
					Objects.erase(Objects.begin() + i);
			}
		}
		for (int i = 0; i < PickUps.size(); i++)
		{
			if (PickUps[i])
				PickUps[i]->Rotate();
			if (PickUps[i] && RangeDelete(*PickUps[i]))
				PickUps.erase(PickUps.begin() + i);
		}
		CollideObjects();
		NewPlayer.VelX *= 0.99;
		NewPlayer.VelY *= 0.99;
		sleep(milliseconds(16));
	}
}

void Engine::RotatePlayer()
{
	Vector2f Mouse(Mouse::getPosition());
	float PI = 3.14159265;
	float dx = ResoX / 2 - Mouse.x;
	float dy = ResoY / 2 - Mouse.y;
	float alpha = atan2f(dy, dx) * 180 / PI;
	NewPlayer.setAngle(alpha - 90);
	NewPlayer.ObjectSprite.setRotation(alpha - 90);
	NewPlayer.Collidor = NewPlayer.ObjectSprite.getGlobalBounds();
}

void Engine::MoveScreenCollidor()
{
		Camera.move(NewPlayer.VelX, NewPlayer.VelY);
		ScreenRes.move(NewPlayer.VelX, NewPlayer.VelY);
		Collidor = ScreenRes.getGlobalBounds();
}

void Engine::CollideObjects()
{
		for (int i = 0; i < (int)Objects.size(); i++)
		{
			if (Objects[i] && Objects[i]->Type == ALIEN && Objects[i]->HP <= 0)
			{
					NewPlayer.CurrentExp += (Objects[i]->MaxHP / 10);
					int x = rand() % 100 + 1;
					if (x <= 8)
					{
						PickUps.push_back(PickUp::createNewPickUp(Objects[i]->ObjectSprite.getPosition().x, Objects[i]->ObjectSprite.getPosition().y, LoadTextures.RocketPickupTex, MisslePick));
					}
					if (x <= 16 && x > 8)
					{
						PickUps.push_back(PickUp::createNewPickUp(Objects[i]->ObjectSprite.getPosition().x, Objects[i]->ObjectSprite.getPosition().y, LoadTextures.KitTex, Kit));
					}
					Objects.erase(Objects.begin() + i);
			}
			for (int j = 0; j < (int)Objects.size() - 1; j++)
			{
				if (Objects[j] && Objects[i] && Objects[j]->Type == ALIEN && Objects[i]->Type == MISSLE && Objects[i]->Collidor.intersects(Objects[j]->Collidor) && Objects[i]->deltaTime < 2.4 && i != j)
				{
					Objects[i]->deltaTime = 2.4;
					Objects[j]->HP -= NewPlayer.Damage;
				}
			}
			if (Objects[i] && Objects[i]->Type == ALIENMISSLE && Objects[i]->Collidor.intersects(NewPlayer.Collidor) && Objects[i]->deltaTime < 2.4)
			{
				Objects[i]->deltaTime = 2.4;
				NewPlayer.HP -= Objects[i]->Damage;
				NewPlayer.CurrentRegenTime = NewPlayer.RegenTime;
			}
		}
		for (int i = 0; i < PickUps.size(); i++)
		{
			if (PickUps[i] && PickUps[i]->Type == MisslePick && PickUps[i]->Collidor.intersects(NewPlayer.Collidor))
			{
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 0, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 15, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 30, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 45, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 60, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 75, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 90, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 105, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 120, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 135, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 150, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 165, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 180, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 195, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 210, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 225, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 240, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 255, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 270, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 285, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 300, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 315, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 330, NewPlayer.Damage, LoadTextures.MissleTex));
				Objects.push_back(GameObject::createNewObject(MISSLE, NewPlayer.ObjectSprite.getPosition().x, NewPlayer.ObjectSprite.getPosition().y, 345, NewPlayer.Damage, LoadTextures.MissleTex));
				PickUps.erase(PickUps.begin() + i);
			}
		}
		for (int i = 0; i < PickUps.size(); i++)
		{
			if (PickUps[i] && PickUps[i]->Type == Kit && PickUps[i]->Collidor.intersects(NewPlayer.Collidor))
			{
				NewPlayer.Heal();
				NewPlayer.Heal();
				NewPlayer.Heal();
				PickUps.erase(PickUps.begin() + i);
			}
		}
}

bool Engine::IsInViewRange(GameObject &Objects)
{
	if (Collidor.intersects(Objects.Collidor))
		return true;
	else
		return false;
}

bool Engine::IsInViewRange(PickUp &Objects)
{
	if (Collidor.intersects(Objects.Collidor))
		return true;
	else
		return false;
}

void Engine::CreatingMonsters()
{
	if (!NewPlayer.Collidor.intersects(Objects[0]->Collidor) && ((rand() % 1000) < 4) && MonsterInt < 15)
	{
		int rr = rand() % 4;
		if (rr == 0)
		{
			Objects.push_back(GameObject::createNewObject(ALIEN, NewPlayer.ObjectSprite.getPosition().x + (rand() % 2000 - 1500), NewPlayer.ObjectSprite.getPosition().y + (rand() % 2000 - 1500), NewPlayer.Level, PlayerToEarth(), LoadTextures.AlienTex));
		}
		else if (rr == 1)
		{
			Objects.push_back(GameObject::createNewObject(ALIEN, NewPlayer.ObjectSprite.getPosition().x + (rand() % 2000 - 1500), NewPlayer.ObjectSprite.getPosition().y + (rand() % 2000 + 1500), NewPlayer.Level, PlayerToEarth(), LoadTextures.AlienTex));
		}
		else if (rr == 2)
		{
			Objects.push_back(GameObject::createNewObject(ALIEN, NewPlayer.ObjectSprite.getPosition().x + (rand() % 2000 + 1500), NewPlayer.ObjectSprite.getPosition().y + (rand() % 2000 - 1500), NewPlayer.Level, PlayerToEarth(), LoadTextures.AlienTex));
		}
		else if (rr == 3)
		{
			Objects.push_back(GameObject::createNewObject(ALIEN, NewPlayer.ObjectSprite.getPosition().x + (rand() % 2000 + 1500), NewPlayer.ObjectSprite.getPosition().y + (rand() % 2000 + 1500), NewPlayer.Level, PlayerToEarth(), LoadTextures.AlienTex));
		}
	}
}

void Engine::MonsterAttack()
{
	for (int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i] && Objects[i]->Type == ALIEN && Objects[i]->Reload <= 0 && sqrt((NewPlayer.ObjectSprite.getPosition().x - Objects[i]->ObjectSprite.getPosition().x) * (NewPlayer.ObjectSprite.getPosition().x - Objects[i]->ObjectSprite.getPosition().x) + (NewPlayer.ObjectSprite.getPosition().y - Objects[i]->ObjectSprite.getPosition().y) * (NewPlayer.ObjectSprite.getPosition().y - Objects[i]->ObjectSprite.getPosition().y)) < 450)
		{
			Objects.push_back(GameObject::createNewObject(ALIENMISSLE, Objects[i]->ObjectSprite.getPosition().x, Objects[i]->ObjectSprite.getPosition().y, (Objects[i]->ObAngle+180), ((int)Objects[i]->HP/10), LoadTextures.MissleTex));
			//Objects.push_back(GameObject::createNewObject(ALIENMISSLE, Objects[i]->ObjectSprite.getPosition().x, Objects[i]->ObjectSprite.getPosition().y, LoadTextures.MissleTex, (Objects[i]->ObAngle + 165)));
			//Objects.push_back(GameObject::createNewObject(ALIENMISSLE, Objects[i]->ObjectSprite.getPosition().x, Objects[i]->ObjectSprite.getPosition().y, LoadTextures.MissleTex, (Objects[i]->ObAngle + 195)));
			Objects[i]->Reload = Objects[i]->ReloadTime;
		}
	}
}

int Engine::PlayerToEarth()
{
	return sqrt((NewPlayer.ObjectSprite.getPosition().x - Objects[0]->ObjectSprite.getPosition().x)*(NewPlayer.ObjectSprite.getPosition().x - Objects[0]->ObjectSprite.getPosition().x) + (NewPlayer.ObjectSprite.getPosition().y - Objects[0]->ObjectSprite.getPosition().y) * (NewPlayer.ObjectSprite.getPosition().y - Objects[0]->ObjectSprite.getPosition().y));
}

bool Engine::RangeDelete(GameObject &Objects)
{
	if (sqrt((NewPlayer.ObjectSprite.getPosition().x - Objects.ObjectSprite.getPosition().x)*(NewPlayer.ObjectSprite.getPosition().x - Objects.ObjectSprite.getPosition().x) + (NewPlayer.ObjectSprite.getPosition().y - Objects.ObjectSprite.getPosition().y) * (NewPlayer.ObjectSprite.getPosition().y - Objects.ObjectSprite.getPosition().y)) > 3500)
		return true;
	else
		return false;
}

bool Engine::RangeDelete(PickUp &PickUps)
{
	if (sqrt((NewPlayer.ObjectSprite.getPosition().x - PickUps.PickUpSprite.getPosition().x)*(PickUps.PickUpSprite.getPosition().x - PickUps.PickUpSprite.getPosition().x) + (PickUps.PickUpSprite.getPosition().y - PickUps.PickUpSprite.getPosition().y) * (PickUps.PickUpSprite.getPosition().y - PickUps.PickUpSprite.getPosition().y)) > 3500)
		return true;
	else
		return false;
}