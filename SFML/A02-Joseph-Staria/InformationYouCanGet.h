#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TankMap.h"
#include "Tank.h"
#include "UpgradeItem.h"
#include "Bullet.h"

using namespace std;
using namespace sf;

class Tank;
class TankMap;

class InformationYouCanGet {
private:
    Tank* m_tank_ptr;

    vector<int> a;
    vector<Vector2f> m_nearby_enenmy_tank_position;
    vector<Vector2f> m_nearby_item_position;
    vector<UpgradeItem::ITEM_TYPE> m_nearby_item_type;

    // the position array of the nearby bullet
    vector<Vector2f> m_nearby_bullet_position;

    const float m_view_range = 960.0f;

    float distance(Vector2f v1, Vector2f v2);

#pragma region Informations
public:
    /*************************************
    my tank information
    *************************************/

    // the position of my tank
    Vector2f my_tank_position();
    
    // the index of block of the map my tank in 
    Vector2u my_tank_map_index();
  
    // the speed of my tank
    float my_tank_speed();
    
    // the health of my tank
    float my_tank_health();
 
    // the maximum health of my tank
    float my_tank_max_health();
   
    // the reload time of the turret of my tank
    float my_turret_reload_time();
    
    // the rotation of the turret
    float my_turret_rotation();

    /*************************************
     map information
     *************************************/

    // the size of the map, default is 80 by 80
    Vector2u map_size() { return TankMap::s_instance->GetMapSize(); }

    // the min index of the row that has no fog
    int map_no_fog_min_row() { return TankMap::s_instance->GetFogPercentage() * map_size().x; }
    
    // the max index of the row that has no fog
    int map_no_fog_max_row() { return (1.0f - TankMap::s_instance->GetFogPercentage()) * map_size().x; }
    
    // the min index of the column that has no fog
    int map_no_fog_min_column() { return TankMap::s_instance->GetFogPercentage() * map_size().y; }
    
    // the max index of the column that has no fog
    int map_no_fog_max_column() { return (1.0f - TankMap::s_instance->GetFogPercentage()) * map_size().y; }
    
    // how many seconds left to next fog growing
    float map_time_to_next_fog_growing() { return TankMap::s_instance->Time_to_next_fog_growing(); }

    /*************************************
    // nearby information
    // range = 10 * 10
     *************************************/

    // view range
    float nearby_view_range() { return m_view_range; }

    // an array of the nearby enemies' positions
    vector<Vector2f> nearby_enenmy_tank_position() { return m_nearby_enenmy_tank_position; }

    // an array of the nearby items' positions
    vector<Vector2f> nearby_item_position() { return m_nearby_item_position; }

    // an array of the nearby items' type
    // if value == UpgradeItem::ITEM_TYPE::ENGINE, the item can upgrade the engine by 1 level
    // if value == UpgradeItem::ITEM_TYPE::ARMOR, the item can upgrade the armor by 1 level
    // if value == UpgradeItem::ITEM_TYPE::TURRET, the item can upgrade the turret by 1 level
    // if value == UpgradeItem::ITEM_TYPE::HEART, the item can heal the health points by 50
    // if value == UpgradeItem::ITEM_TYPE::STAR, the item can upgrade all by 1 level
    vector<UpgradeItem::ITEM_TYPE> nearby_item_type() { return m_nearby_item_type; }

    // an array of the nearby bullets' positions
    vector<Vector2f> nearby_bullet_position() { return m_nearby_bullet_position; }
#pragma endregion

#pragma region FunctionsYouCanUse 
    
    // Get the map index from a given position
    Vector2u PositionToMapIndex(Vector2f position) { return TankMap::s_instance->GetIndexByPosition(position); }

    // Get the position from a given map index
    Vector2f MapIndexToPosition(Vector2u index) { return TankMap::s_instance->GetCenterPositionByIndex(index); }

    // Get the terrain from a given index
    Terrain GetTerrain_from_MapIndex(Vector2u index) { return TankMap::s_instance->GetTerrainByIndex(index.x, index.y); }
#pragma endregion


public:
    InformationYouCanGet(Tank* my_tank);
    // update information
    void Update(vector<Tank*> all_tank, vector<UpgradeItem*> all_item, vector<Bullet*> all_bullet);
};

