#include "TankMap.h"
#include <fstream>


TankMap* TankMap::s_instance = nullptr;

TankMap::TankMap(string path)
{
    s_instance = this;
    LoadMap(path);
    LoadTerrain();
    InitSprite("Images/");
}


Vector2u TankMap::GetMapSize()
{
    return Vector2u(m_row_n, m_col_n);
}

Vector2f TankMap::GetMapSizePixel()
{
    return Vector2f(m_col_n * m_cell_size.x, m_row_n * m_cell_size.y);
}

Vector2f TankMap::GetCornerPositionByIndex(int i, int j)
{
    return Vector2f(j * m_cell_size.x, i * m_cell_size.y);
}

Vector2f TankMap::GetCornerPositionByIndex(Vector2u index)
{
    return GetCornerPositionByIndex(index.x, index.y);
}

Vector2f TankMap::GetCenterPositionByIndex(int i, int j)
{
    return GetCornerPositionByIndex(i, j) + m_cell_size * 0.5f;
}

Vector2f TankMap::GetCenterPositionByIndex(Vector2u index)
{
    return GetCenterPositionByIndex(index.x, index.y);
}

Vector2u TankMap::GetIndexByPosition(Vector2f position)
{
    int j = position.x / m_cell_size.x; // get the cell index j
    int i = position.y / m_cell_size.y; // get the cell index i
    return Vector2u(i, j);
}

Terrain TankMap::GetTerrainByIndex(int i, int j)
{
    int type = m_map_data[i * m_col_n + j]; // the type of the cell
    return m_terrains[type];
}


bool TankMap::CheckInFogWithPosition(Vector2f position)
{
    int j = position.x / m_cell_size.x; // get the cell index j
    int i = position.y / m_cell_size.y; // get the cell index i

    if (i < 0 || j < 0 || i >= m_row_n || j >= m_col_n) // if it is out of range
        return true;
    return CheckInFogWithIndex(i, j);
}

bool TankMap::CheckInFogWithIndex(int i, int j)
{
    if (i < m_fog.percent * m_row_n)
        return true;
    else if (i >= (1.0f - m_fog.percent) * m_row_n)
        return true;
    else if (j < m_fog.percent * m_col_n)
        return true;
    else if (j >= (1.0f - m_fog.percent) * m_col_n)
        return true;
    return false;
}

void TankMap::LoadTerrain()
{
    m_terrains[0].name = "dirt1";
    m_terrains[0].tank_passable = true;
    m_terrains[0].bullet_passable = true;

    m_terrains[1].name = "stone1";
    m_terrains[1].tank_passable = false;
    m_terrains[1].bullet_passable = false;

    m_terrains[2].name = "water";
    m_terrains[2].tank_passable = false;
    m_terrains[2].bullet_passable = true;

    m_terrains[3].name = "dirt2";
    m_terrains[3].tank_passable = true;
    m_terrains[3].bullet_passable = true;

    m_terrains[4].name = "dirt3";
    m_terrains[4].tank_passable = true;
    m_terrains[4].bullet_passable = true;

    m_terrains[5].name = "grass";
    m_terrains[5].tank_passable = true;
    m_terrains[5].bullet_passable = true;
}

void TankMap::LoadMap(string path)
{
    ifstream inf;
    inf.open(path); // open the file


    inf >> m_row_n; // read the number of the rows
    inf >> m_col_n; // read the number of the columns

    m_map_data = new int[m_row_n * m_col_n]; // allocate memory to save the data

     // read the map data from file
    for (int i = 0; i < m_row_n; i++) {
        for (int j = 0; j < m_col_n; j++) {
            inf >> m_map_data[i * m_col_n + j];
        }
    }

    inf.close();    // close the file
}

void TankMap::InitSprite(string path_to_folder)
{
    for (int i = 0; i < TERRAIN_TYPE_NUMBER; i++) {
        Texture* texture = new Texture();
        texture->loadFromFile(path_to_folder + m_terrains[i].name + ".png");
        m_terrains[i].sprite.setTexture(*texture);
        m_terrains[i].sprite.setScale((float)m_cell_size.x / texture->getSize().x, (float)m_cell_size.y / texture->getSize().y);
    }
}

bool TankMap::CheckPositionTankPassable(Vector2f position)
{
    int j = position.x / m_cell_size.x; // get the cell index j
    int i = position.y / m_cell_size.y; // get the cell index i

    if (i < 0 || j < 0 || i >= m_row_n || j >= m_col_n) // if it is out of range
        return false;

    int type = m_map_data[i * m_col_n + j]; // get the type

    // return if the terrain can be passed or not 
    return m_terrains[type].tank_passable;
}

bool TankMap::CheckRectTankPassable(Vector2f position, Vector2f size)
{
    if (CheckPositionTankPassable(position + 0.5f * size) == false)
        return false;
    else if (CheckPositionTankPassable(position - 0.5f * size) == false)
        return false;
    else if (CheckPositionTankPassable(position + Vector2f(-0.5f * size.x, 0.5f * size.y)) == false)
        return false;
    else if (CheckPositionTankPassable(position + Vector2f(0.5f * size.x, -0.5f * size.y)) == false)
        return false;
    return true;
}

bool TankMap::CheckPositionBulletPassable(Vector2f position)
{
    int x = position.x / m_cell_size.x;
    int y = position.y / m_cell_size.y;

    if (x < 0 || y < 0 || x >= m_row_n || y >= m_col_n)
        return false;

    int type = m_map_data[y * m_col_n + x];

    // return if the terrain can be passed or not 
    return m_terrains[type].bullet_passable;
}

void TankMap::Update(RenderWindow& window, float deltaSeconds)
{
    m_fog.Update(window, deltaSeconds);
}

void TankMap::Render(RenderWindow& window)
{
    for (int i = 0; i < m_row_n; i++) {
        for (int j = 0; j < m_col_n; j++) {
            int type = m_map_data[i * m_col_n + j]; // the type of the cell
            Vector2f position = GetCornerPositionByIndex(i, j); // the position of the cell
            m_terrains[type].sprite.setPosition(position); // set the position of the sprite

            // if (CheckInFogWithIndex(i, j))
               // m_terrains[type].sprite.setColor(m_fog.fog_color);
            // else
                m_terrains[type].sprite.setColor(Color::White);

            window.draw(m_terrains[type].sprite); // draw the sprite
        }
    }

    m_fog.Render(window);
}

PoisonousFog::PoisonousFog()
{
    m_rect_up.setFillColor(fog_color);
    m_rect_left.setFillColor(fog_color);
    m_rect_right.setFillColor(fog_color);
    m_rect_down.setFillColor(fog_color);
}

void PoisonousFog::Update(RenderWindow& window, float deltaSeconds)
{
    if (percent >= 0.5f)
        return;
    if (m_is_growing == true) {
        percent += m_speed * deltaSeconds;
        if (percent >= m_target) {
            percent = m_target;
            m_is_growing = false;
            m_wait_time_left = m_wait_time_max;
        }
        float map_x = TankMap::s_instance->GetMapSizePixel().x;
        float map_y = TankMap::s_instance->GetMapSizePixel().y;
        
        m_rect_up.setSize(Vector2f(map_x, percent * map_y));

        m_rect_left.setPosition(0, percent * map_y);
        m_rect_left.setSize(Vector2f(percent * map_x, (1.0f - 2.0f * percent) * map_y));

        m_rect_right.setPosition((1.0f - percent) * map_y, percent * map_y);
        m_rect_right.setSize(Vector2f(percent * map_x, (1.0f - 2.0f * percent) * map_y));

        m_rect_down.setPosition(0, (1.0f - percent) * map_y);
        m_rect_down.setSize(Vector2f(map_x, percent * map_y));
    }
    else {
        m_wait_time_left -= deltaSeconds;
        if (m_wait_time_left <= 0.0f) {
            m_target = percent + m_percent_step;
            m_is_growing = true;
        }
    }
}

void PoisonousFog::Render(RenderWindow& window)
{
    window.draw(m_rect_up);
    window.draw(m_rect_left);
    window.draw(m_rect_right);
    window.draw(m_rect_down);
}
