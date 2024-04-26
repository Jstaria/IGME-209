#include "InformationYouCanGet.h"

InformationYouCanGet::InformationYouCanGet(Tank* my_tank) {
	m_tank_ptr = my_tank;
}

void InformationYouCanGet::Update(vector<Tank*> all_tank, vector<UpgradeItem*> all_item, vector<Bullet*> all_bullet)
{
	// update nearby tank
	m_nearby_enenmy_tank_position.clear();

	for (int i = 0; i < all_tank.size(); i++) {
		Tank* tank = all_tank[i];
		if (tank->is_active == false || tank->GetID() == m_tank_ptr->GetID()) {
			continue;
		}
		
		if (this->distance(m_tank_ptr->GetPosition(), tank->GetPosition()) < nearby_view_range()) {
			m_nearby_enenmy_tank_position.push_back(tank->GetPosition());
		}
	}

	// update nearby item
	m_nearby_item_position.clear();
	m_nearby_item_type.clear();

	for (int i = 0; i < all_item.size(); i++) {
		UpgradeItem* item = all_item[i];
		if (item->is_active == false) {
			continue;
		}

		if (this->distance(m_tank_ptr->GetPosition(), item->sprite.getPosition()) < nearby_view_range()) {
			m_nearby_item_position.push_back(item->sprite.getPosition());
			m_nearby_item_type.push_back(item->item_type);
		}
	}

	// update nearby bullet
	// update nearby item
	m_nearby_bullet_position.clear();

	for (int i = 0; i < all_bullet.size(); i++) {
		Bullet* bullet = all_bullet[i];
		if (bullet->is_active == false || bullet->GetTankID() == m_tank_ptr->GetID()) {
			continue;
		}

		if (this->distance(m_tank_ptr->GetPosition(), bullet->GetPosition()) < nearby_view_range()) {
			m_nearby_bullet_position.push_back(bullet->GetPosition());
		}
	}
}

float InformationYouCanGet::distance(Vector2f v1, Vector2f v2)
{
	return sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

Vector2f InformationYouCanGet::my_tank_position()
{
	return m_tank_ptr->GetPosition();
}

Vector2u InformationYouCanGet::my_tank_map_index()
{
	return TankMap::s_instance->GetIndexByPosition(this->my_tank_position());
}

float InformationYouCanGet::my_tank_speed()
{
	return m_tank_ptr->GetSpeed();
}

float InformationYouCanGet::my_tank_health()
{
	return m_tank_ptr->GetHealth();
}

float InformationYouCanGet::my_tank_max_health()
{
	return m_tank_ptr->GetMaxHealth();
}

float InformationYouCanGet::my_turret_reload_time()
{
	return m_tank_ptr->GetReloadTime();
}

float InformationYouCanGet::my_turret_rotation()
{
	return m_tank_ptr->GetTurretRotatoin();
}
