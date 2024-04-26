#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TankMap.h"
#include "TankController.h"
#include "Bullet.h"
#include "UpgradeItem.h"

using namespace std;
using namespace sf;

class TankController;

class Tank {
public:
	bool is_active = true;

private:
	Sprite m_baseSprite;
	Sprite m_turretSprite;

	float m_move_speed = 50.0f;
	float m_turret_rotate_speed = 180.0f;
	int m_health = 100;
	int m_max_health = 100;

	TankController* m_controller = nullptr;

	float m_turretReloadTotalTime = 1.0f;
	float m_turretReloadLeftTime = 0.0f;

	float const speed_upgrade_one_level = 5.0f;
	int const hp_upgrade_one_level = 10;
	float const reload_upgrade_one_level = 0.05f;

	float const speed_upgrade_limit = 100.0f;
	int const hp_upgrade_limit = 200;
	float const reload_upgrade_limit = 0.5f;

	int m_enginee_upgrade_time = 0;
	int m_armor_upgrade_time = 0;
	int m_turret_upgrade_time = 0;

	
public:
	int GetEngineUpgrade() { return m_enginee_upgrade_time; }
	int GetArmorUpgrade() { return m_armor_upgrade_time; }
	int GetTurretUpgrade() { return m_turret_upgrade_time; }
	int kills = 0;


private:
	// circle bounding volume
	float m_bounding_radius = 32.0f;

	Vector2f m_tankSize = Vector2f(64, 64);

	int m_tankID;

	Sprite m_health_sprite;
	Sprite m_health_empty_sprite;

	Vector2f m_health_bar_size = Vector2f(48, 4);
	Vector2f m_health_bar_offset = Vector2f(-24, -40);

	float m_in_fog_time = 0.0f;

	// name of my tank, used to display
	string m_tank_name;
	Text m_name_text;
	Font font;

public:
	int total_hit_get = 0;

public:
	Tank(Color color, Vector2f position, int tankID, string name);
	~Tank();
	void Render(RenderWindow& window);
	void Update(RenderWindow& window, float deltaSeconds);
	void SetController(TankController* controller);

	Vector2f GetPosition();
	bool CheckGetHit(Bullet* bullet);
	bool CheckEatItem(UpgradeItem* item);
	

	void UpgradeTank(int engine, int armor, int turret);

	string GetName() { return m_tank_name; }
	void SetName(string name);
	float GetSpeed() { return m_move_speed; }
	float GetHealth() { return m_health; }
	float GetMaxHealth() { return m_max_health; }
	float GetReloadTime() { return m_turretReloadTotalTime; }
	float GetTurretRotatoin() { return m_turretSprite.getRotation(); }
	int GetID() { return m_tankID; }
	TankController* GetController() { return m_controller; }

private:
	void GetHit(int power);
	void Heal(int power);
	void UpgradeEngine(int n);
	void UpgradeArmor(int n);
	void UpgradeTurret(int n);
	void CheckInPosionFog(float deltaSeconds);
	void Fire();
};