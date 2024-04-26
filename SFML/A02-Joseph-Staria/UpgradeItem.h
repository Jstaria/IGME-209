#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "TankMap.h"
#include <string>

using namespace std;
using namespace sf;

#define ITEM_TYPE_NUMBER 5

class UpgradeItem
{
public:
	Sprite sprite;
	float r = 32.0f;
	bool is_active = true;
	enum class ITEM_TYPE {
		ENGINE = 0,
		ARMOR = 1,
		TURRET = 2,
		HEART = 3,
		STAR = 4
	};
	UpgradeItem::ITEM_TYPE item_type;
};

class UpgradeItemManager 
{
private:
	float m_item_probability = 0.05f;
	float m_last_time = 5.0f;
	float m_max_last_time = 20.0f;

	string m_texture_name[ITEM_TYPE_NUMBER] = { "engine", "armor", "turret", "heart", "star"};
	vector<Texture*> texture_array;

public:
	UpgradeItemManager();
	vector<UpgradeItem*> item_array;
	void Update(RenderWindow& window, float deltaSeconds);
	void Render(RenderWindow& window);

private:
	void GenerateNewItems();
};

