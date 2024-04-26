#include "UpgradeItem.h"

UpgradeItemManager::UpgradeItemManager()
{
	for (int i = 0; i < ITEM_TYPE_NUMBER; i++) {
		Texture* texture = new Texture();
		texture->loadFromFile("Images/" + m_texture_name[i] + ".png");

		texture_array.push_back(texture);
	}
}

void UpgradeItemManager::Update(RenderWindow& window, float deltaSeconds)
{
	m_last_time -= deltaSeconds;
	if (m_last_time < 0.0f) {
		GenerateNewItems();
		m_last_time = m_max_last_time;
	}
}

void UpgradeItemManager::Render(RenderWindow& window)
{
	for (int i = 0; i < item_array.size(); i++) {
		if (item_array[i]->is_active == true)
			window.draw(item_array[i]->sprite);
	}
}

void UpgradeItemManager::GenerateNewItems()
{
	for (int i = 0; i < item_array.size(); i++) {
		delete item_array[i];
		item_array[i] = nullptr;
	}
	item_array.clear();

	Vector2u mapSize = TankMap::s_instance->GetMapSize();
	float fog_percentage = TankMap::s_instance->GetFogPercentage();
	int min_i = fog_percentage * mapSize.x;
	int max_i = (1.0f - fog_percentage) * mapSize.x;
	int min_j = fog_percentage * mapSize.y;
	int max_j = (1.0f - fog_percentage) * mapSize.y;

	for (int i = min_i; i < max_i; i++) {
		for (int j = min_j; j < max_j; j++) {
			if (TankMap::s_instance->GetTerrainByIndex(i, j).tank_passable == true) {
				float rand_n = 1.0f * (rand() % 100) / 100.0f;
				if (rand_n < m_item_probability) {
					int type = rand() % ITEM_TYPE_NUMBER;
					UpgradeItem* item_ptr = new UpgradeItem();
					item_ptr->sprite.setTexture(*texture_array[type]);
					item_ptr->sprite.setOrigin(12, 12);
					item_ptr->sprite.setPosition(TankMap::s_instance->GetCenterPositionByIndex(i, j));
					item_ptr->sprite.setScale(2.0f, 2.0f);
					item_ptr->item_type = (UpgradeItem::ITEM_TYPE)type;

					item_array.push_back(item_ptr);
				}
			}
		}
	}
}
