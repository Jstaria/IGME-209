#include "Tank.h"
#include "ViewManager.h"

Tank::Tank(Color color, Vector2f position, int tankID, string name)
{
    // create textures
    Texture* baseTexture_ptr = new Texture();
    baseTexture_ptr->loadFromFile("Images/tankBase.png"); // load tank base textures
    Texture* turretTexture_ptr = new Texture();
    turretTexture_ptr->loadFromFile("Images/tankTurret.png"); // load tank turret textures

    m_baseSprite.setTexture(*baseTexture_ptr); // assign the base texture to base sprite
    m_turretSprite.setTexture(*turretTexture_ptr); // assign the turretTexture to turret sprite

    // init origin
    m_baseSprite.setOrigin(baseTexture_ptr->getSize().x / 2.0f, baseTexture_ptr->getSize().y / 2.0f);
    m_turretSprite.setOrigin(turretTexture_ptr->getSize().x / 2.0f, turretTexture_ptr->getSize().y / 2.0f);

    // init position
    m_baseSprite.setPosition(position);
    m_turretSprite.setPosition(position);

    // init rotation
    m_baseSprite.setRotation(0);
    m_turretSprite.setRotation(0);

    // init scale
    m_baseSprite.setScale(m_tankSize.x / baseTexture_ptr->getSize().x, m_tankSize.y / baseTexture_ptr->getSize().y);
    m_turretSprite.setScale(m_tankSize.x / baseTexture_ptr->getSize().x, m_tankSize.y / baseTexture_ptr->getSize().y);

    // set color
    m_baseSprite.setColor(color);
    m_turretSprite.setColor(color);

   
    Texture* healthTexture_ptr = new Texture();
    healthTexture_ptr->loadFromFile("Images/health.png");
    Texture* healthEmptyTexture_ptr = new Texture();
    healthEmptyTexture_ptr->loadFromFile("Images/health_empty.png");

    m_health_sprite.setTexture(*healthTexture_ptr);
    m_health_empty_sprite.setTexture(*healthEmptyTexture_ptr);

    m_health_sprite.setScale((float)m_health / m_max_health * m_health_bar_size.x / healthTexture_ptr->getSize().x, 
        m_health_bar_size.y / healthTexture_ptr->getSize().y);
    m_health_empty_sprite.setScale(m_health_bar_size.x / healthTexture_ptr->getSize().x, m_health_bar_size.y / healthTexture_ptr->getSize().y);

    m_health_sprite.setPosition(position + m_health_bar_offset);
    m_health_empty_sprite.setPosition(position + m_health_bar_offset);

    m_tankID = tankID;


    // set up name
    m_tank_name = name;

    if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
    {
        // error...
    }
    m_name_text.setFont(font);
    m_name_text.setString(m_tank_name);
    m_name_text.setCharacterSize(m_tankSize.x / 2.0f);
}

Tank::~Tank()
{
}

void Tank::Render(RenderWindow& window)
{
    window.draw(m_baseSprite);// draw the base sprite

    if (is_active == true)
        window.draw(m_turretSprite);// draw the base sprite 

    window.draw(m_health_empty_sprite);
    window.draw(m_health_sprite);

    //sf::Font font;
    //if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
    //{
    //    // error...
    //}

    //m_name_text.setFont(font);
    //m_name_text.setString("hello world");
    //m_name_text.setCharacterSize(m_tankSize.x);

    window.draw(m_name_text);
}

void Tank::Update(RenderWindow& window, float deltaSeconds)
{
    if (is_active == false)
        return;

    m_controller->Update(window, deltaSeconds);

    Vector2f movement = Vector2f(0, 0);
    float rotation = m_baseSprite.getRotation();

    if (m_controller->direction == TankController::MOVE_DIRECTION::UP) {
        rotation = 0;
        movement.y -= m_move_speed * deltaSeconds;
    }
    else if (m_controller->direction == TankController::MOVE_DIRECTION::DOWN) {
        rotation = 180;
        movement.y += m_move_speed * deltaSeconds;
    }
    else if (m_controller->direction == TankController::MOVE_DIRECTION::LEFT) {
        rotation = -90;
        movement.x -= m_move_speed * deltaSeconds;
    }
    else if (m_controller->direction == TankController::MOVE_DIRECTION::RIGHT) {
        rotation = 90;
        movement.x += m_move_speed * deltaSeconds;
    }

    Vector2f newPosition = m_baseSprite.getPosition() + movement;
    if (TankMap::s_instance->CheckRectTankPassable(newPosition, Vector2f(m_baseSprite.getTexture()->getSize()) * m_baseSprite.getScale().x)) {
        m_baseSprite.setPosition(newPosition);
        m_turretSprite.setPosition(newPosition);

        m_health_sprite.setPosition(newPosition + m_health_bar_offset);
        m_health_empty_sprite.setPosition(newPosition + m_health_bar_offset);
        m_name_text.setPosition(newPosition + Vector2f(0.0f, 30.0f));
    }

    // set rotation
    m_baseSprite.setRotation(rotation);

    if (m_controller->rotate_direction == TankController::ROTATE_DIRECTION::CLOCKWISE) {
        m_turretSprite.rotate(m_turret_rotate_speed * deltaSeconds);
    }
    else if (m_controller->rotate_direction == TankController::ROTATE_DIRECTION::COUNTER_CLOCKWISE) {
        m_turretSprite.rotate(-m_turret_rotate_speed * deltaSeconds);
    }
    // m_turretSprite.setRotation(rotation);

    // fire!
    m_turretReloadLeftTime -= deltaSeconds;
    if (m_controller->is_fire_active) {
        this->Fire();
    }

    // check fog
    CheckInPosionFog(deltaSeconds);
}

void Tank::SetController(TankController* controller)
{
    m_controller = controller;
}

Vector2f Tank::GetPosition()
{
    return m_baseSprite.getPosition();
}

float distance(Vector2f v1, Vector2f v2) {
    return sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

bool Tank::CheckGetHit(Bullet* bullet)
{
    if (this->is_active == false)
        return false;
    if (bullet->GetTankID() == m_tankID)
        return false;
    else {
        if (distance(this->GetPosition(), bullet->GetPosition()) < m_bounding_radius) { // a sphere bounding volume
            this->GetHit(bullet->GetPower());

            // killed
            if (this->is_active == false) {
                bullet->m_tank->kills++;
                ViewManager::s_instance->AddKill(bullet->m_tank->GetName(), this->m_tank_name);
                // cout << bullet->m_tank->GetName() << " kills " << m_tank_name << endl;
            }
            // set the active of bullet to be false
            bullet->is_active = false;
            return true;
        }
    }
}

bool Tank::CheckEatItem(UpgradeItem* item)
{
    if (this->is_active == false)
        return false;
    if (item->is_active == true) {
        if (distance(this->GetPosition(), item->sprite.getPosition()) < m_bounding_radius + item->r) { // a sphere bounding volume
            item->is_active = false;

            switch (item->item_type) {
            case UpgradeItem::ITEM_TYPE::ENGINE:
                UpgradeEngine(1); break;
            case UpgradeItem::ITEM_TYPE::ARMOR:
                UpgradeArmor(1); break;
            case UpgradeItem::ITEM_TYPE::TURRET:
                UpgradeTurret(1); break;
            case UpgradeItem::ITEM_TYPE::HEART:
                Heal(50); break;
                break;
            case UpgradeItem::ITEM_TYPE::STAR:
                UpgradeEngine(1); UpgradeArmor(1); UpgradeTurret(1); break;
            default:
                break;
            }

            return true;
        }
    }
    return false;
}

void Tank::GetHit(int power)
{
    m_health -= power;

    if (m_health > 0)
        total_hit_get += power;

    if (m_health <= 0) { // life <= 0
        m_health = 0;
        is_active = false;
        m_baseSprite.setColor(Color(50, 50, 50));
    }

    // update the health bar
    m_health_sprite.setScale((float)m_health / m_max_health * m_health_bar_size.x / m_health_sprite.getTexture()->getSize().x,
        m_health_bar_size.y / m_health_sprite.getTexture()->getSize().y);
}

void Tank::Heal(int power)
{
    m_health += power;
    if (m_health > m_max_health) { // life <= 0
        m_health = m_max_health;
    }

    // update the health bar
    m_health_sprite.setScale((float)m_health / m_max_health * m_health_bar_size.x / m_health_sprite.getTexture()->getSize().x,
        m_health_bar_size.y / m_health_sprite.getTexture()->getSize().y);
}

void Tank::UpgradeEngine(int n)
{ 
    m_move_speed += speed_upgrade_one_level * n;

    m_enginee_upgrade_time += n;

    if (m_enginee_upgrade_time > 10)
        m_enginee_upgrade_time = 10;

    if (m_move_speed > speed_upgrade_limit)
        m_move_speed = speed_upgrade_limit;

    // cout << m_move_speed << " " << m_health << " " << m_max_health << " " << m_turretRoloadTotalTime << endl;
}

void Tank::UpgradeArmor(int n)
{
    m_max_health += hp_upgrade_one_level * n;
    m_health += hp_upgrade_one_level * n;

    m_armor_upgrade_time += n;

    if (m_armor_upgrade_time > 10)
        m_armor_upgrade_time = 10;

    if (m_max_health > hp_upgrade_limit)
        m_max_health = hp_upgrade_limit;

    if (m_health > hp_upgrade_limit)
        m_max_health = hp_upgrade_limit;

    // cout << m_move_speed << " " << m_health << " " << m_max_health << " " << m_turretRoloadTotalTime << endl;
}

void Tank::UpgradeTurret(int n)
{
    m_turretReloadTotalTime -= reload_upgrade_one_level * n;
    if (m_turretReloadTotalTime < reload_upgrade_limit) {
        m_turretReloadTotalTime = reload_upgrade_limit;
    }

    m_turret_upgrade_time += n;

    if (m_turret_upgrade_time > 10)
        m_turret_upgrade_time = 10;

    // cout << m_move_speed << " " << m_health << " " << m_max_health << " " << m_turretRoloadTotalTime << endl;
}

void Tank::CheckInPosionFog(float deltaSeconds)
{
    if (this->is_active == true && TankMap::s_instance->CheckInFogWithPosition(this->GetPosition())) {
        m_in_fog_time += deltaSeconds;
        if (m_in_fog_time >= 0.1f) {
            GetHit(1);

            if (this->is_active == false) {
                ViewManager::s_instance->AddKill("fog", this->m_tank_name);
            }

            m_in_fog_time -= 0.1f;           
        }
    }
}

void Tank::UpgradeTank(int engine, int armor, int turret)
{
    if (engine + armor + turret > 10)
        return;

    UpgradeEngine(engine);
    UpgradeArmor(armor);
    UpgradeTurret(turret); 
}

void Tank::SetName(string name)
{
    m_tank_name = name; m_name_text.setString(m_tank_name);
    m_name_text.setOrigin(m_tank_name.length() * 8, 0.0f);

    /*Texture* base = new Texture();
    Texture* turret = new Texture();*/

    //if (base->loadFromFile("Images/Customized/" + name + "_base.png")) {
    //    Sprite* sprite = new Sprite();
    //    sprite->setTexture(*base); // assign the base texture to base sprite
    //    sprite->setOrigin(base->getSize().x / 2.0f, base->getSize().y / 2.0f);
    //    sprite->setPosition(m_baseSprite.getPosition());
    //    sprite->setScale(m_tankSize.x / base->getSize().x, m_tankSize.y / base->getSize().y);    // set scale
    //    m_baseSprite = *sprite;
    //    if (turret->loadFromFile("Images/Customized/" + name + "_turret.png")) {
    //        Sprite* sprite_turret = new Sprite();
    //        sprite_turret->setTexture(*turret); // assign the base texture to base sprite
    //        sprite_turret->setOrigin(turret->getSize().x / 2.0f, turret->getSize().y / 2.0f);
    //        sprite_turret->setPosition(m_turretSprite.getPosition());
    //        sprite_turret->setScale(m_tankSize.x / turret->getSize().x, m_tankSize.y / turret->getSize().y);    // set scale
    //        m_turretSprite = *sprite_turret;
    //    }
    //    else {
    //        m_turretSprite.setColor(Color(255, 255, 255, 0));
    //    }
    //}
}


void Tank::Fire()
{
    if (m_turretReloadLeftTime <= 0.0f) {
        BulletManager::s_instance->AddBulletToScene(new Bullet(m_turretSprite.getPosition(), m_turretSprite.getRotation(), m_tankID, this));
        m_turretReloadLeftTime = m_turretReloadTotalTime;
    }
}
