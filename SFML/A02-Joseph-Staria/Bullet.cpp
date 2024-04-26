#include "Bullet.h"

BulletManager* BulletManager::s_instance = nullptr;

BulletManager::BulletManager()
{
    bulletTexture_ptr = new Texture();
    bulletTexture_ptr->loadFromFile("Images/bullet.png");

    s_instance = this;
}

void BulletManager::Render(RenderWindow& window)
{
    for (int i = 0; i < bullet_array.size(); i++) {
        bullet_array[i]->Render(window);
    }
}

void BulletManager::Update(RenderWindow& window, float deltaSeconds)
{
    // update the bullet and free the memory if the active is false
    for (int i = 0; i < bullet_array.size(); i++) {
        bullet_array[i]->Update(window, deltaSeconds);
        if (bullet_array[i]->is_active == false) {
            delete bullet_array[i];
            bullet_array[i] = nullptr;

            // the bullet object is deleted, and the pointer is nullptr
            // but that nullptr is still in the pointer array 
        }
    }

    // remove the pointer out of the bullet array
    bullet_array.erase(std::remove_if(
        bullet_array.begin(), bullet_array.end(),
        [](const Bullet* bullet) {
            return bullet == nullptr;
        }), bullet_array.end());
}

void BulletManager::AddBulletToScene(Bullet* bullet)
{
    bullet_array.push_back(bullet);
}

Bullet::Bullet(Vector2f position, float rotation, int tankID, Tank* tank)
{
    
    m_sprite.setTexture(*BulletManager::s_instance->bulletTexture_ptr);

    m_sprite.setOrigin(Vector2f(m_sprite.getTexture()->getSize()) / 2.0f);
    m_sprite.setPosition(position);
    m_sprite.setRotation(rotation);

    // here, we show an example to calculate the direction of the bullet with cos and sin of the rotation.
    // the direction can be hard coded in our code since we only have four different directions.
    float rotateArc = (rotation - 90) * 3.1415926f / 180.0f;
    m_direction = Vector2f(cos(rotateArc), sin(rotateArc));

    m_sprite.move(10.0f * m_direction);

    m_tank = tank;
    m_tankID = tankID;
}

Bullet::~Bullet()
{

}

void Bullet::Render(RenderWindow& window)
{
    window.draw(m_sprite);
}

void Bullet::Update(RenderWindow& window, float deltaSeconds)
{
    if (is_active == false)
        return;

    m_sprite.move(m_speed * deltaSeconds * m_direction);

    if (TankMap::s_instance->CheckPositionBulletPassable(m_sprite.getPosition()) == false)
        is_active = false;

    m_life -= deltaSeconds;
    if (m_life <= 0.0f)
        is_active = false;
}

Vector2f Bullet::GetPosition()
{
    return m_sprite.getPosition();
}

int Bullet::GetTankID()
{
    return m_tankID;
}

int Bullet::GetPower()
{
    return m_power;
}
