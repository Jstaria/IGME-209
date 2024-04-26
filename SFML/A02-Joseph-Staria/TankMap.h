#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

#define TERRAIN_TYPE_NUMBER 6

class Terrain {
public:
	string name;
	bool tank_passable;
	bool bullet_passable;
	Sprite sprite;

	bool is_in_fog = false;
};

class PoisonousFog {
public:
	
	float percent = 0.0f;
	Color fog_color = Color(0, 200, 0, 50);

private:
	RectangleShape m_rect_up;
	RectangleShape m_rect_left;
	RectangleShape m_rect_right;
	RectangleShape m_rect_down;

	float const m_speed = 0.005f; // speed of the fog, percent per seconds
	float m_target = 0.0f; // target size
	bool m_is_growing = false;

	float const m_percent_step = 0.1f;
	float const m_wait_time_max = 15.0f; // the fog pauses for x seconds
	float m_wait_time_left = 15.0f;
	
public:
	PoisonousFog();
	void Update(RenderWindow& window, float deltaSeconds);
	void Render(RenderWindow& window);

	float Time_to_next_fog_growing() { 
		if (m_is_growing == true)
			return 0.0f;
		else
			return m_wait_time_left;
	}
};

class TankMap
{
public:
	static TankMap* s_instance;
private:
	int m_row_n;	// number of rows, will be loaded from file
	int m_col_n;	// number of colums, will be loaded from file

	int* m_map_data;	// map data

	Vector2f m_cell_size = Vector2f(96, 96);

	Terrain m_terrains[TERRAIN_TYPE_NUMBER];
	PoisonousFog m_fog;

public:
	TankMap(string path);

	void Update(RenderWindow& window, float deltaSeconds);
	void Render(RenderWindow& window);

	bool CheckPositionTankPassable(Vector2f position);
	bool CheckRectTankPassable(Vector2f position, Vector2f size);
	bool CheckPositionBulletPassable(Vector2f position);

	Vector2u GetMapSize();
	Vector2f GetMapSizePixel();
	Vector2f GetCornerPositionByIndex(int x, int y);
	Vector2f GetCornerPositionByIndex(Vector2u index);
	Vector2f GetCenterPositionByIndex(int i, int j);
	Vector2f GetCenterPositionByIndex(Vector2u index);
	Vector2u GetIndexByPosition(Vector2f position);
	Terrain GetTerrainByIndex(int i, int j);
	
	bool CheckInFogWithPosition(Vector2f position);
	bool CheckInFogWithIndex(int x, int y);

	float GetFogPercentage() {
		return m_fog.percent;
	}

	float Time_to_next_fog_growing() {
		return m_fog.Time_to_next_fog_growing();
	}

private:
	void LoadTerrain();
	void LoadMap(string path);
	void InitSprite(string path_to_folder);
};
