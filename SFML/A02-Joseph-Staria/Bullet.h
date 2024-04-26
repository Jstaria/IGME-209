#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "TankMap.h"

using namespace std;
using namespace sf;

class Tank;

class Bullet
{
public:
	bool is_active = true;
	Tank* m_tank;

private:
	Sprite m_sprite;
	float m_speed = 300.0f;
	Vector2f m_direction = Vector2f(0.0f, -1.0f);
	float m_life = 5.0f;

	int m_tankID; // bullet needs a tank ID to avoid a bullet hits it's own tank

	int m_power = 20;

public:
	Bullet(Vector2f position, float rotation, int tankID, Tank* tank);
	~Bullet();

	void Render(RenderWindow& window);
	void Update(RenderWindow& window, float deltaSeconds);

	Vector2f GetPosition();
	int GetTankID(); // bullet needs a tank ID to avoid a bullet hits it's own tank
	int GetPower();
};

class BulletManager {
public:
	vector<Bullet*> bullet_array;
	static BulletManager* s_instance;

	Texture* bulletTexture_ptr;
public:
	BulletManager();

	void Render(RenderWindow& window);
	void Update(RenderWindow& window, float deltaSeconds);

	void AddBulletToScene(Bullet* bullet); // will be called by the tank
};
