#include "PlayerController.h"

void PlayerController::Update(RenderWindow& window, float deltaSeconds)
{
	if (Keyboard::isKeyPressed(Keyboard::W) == true)
		this->direction = MOVE_DIRECTION::UP;
	else if (Keyboard::isKeyPressed(Keyboard::S) == true)
		this->direction = MOVE_DIRECTION::DOWN;
	else if (Keyboard::isKeyPressed(Keyboard::A) == true)
		this->direction = MOVE_DIRECTION::LEFT;
	else if (Keyboard::isKeyPressed(Keyboard::D) == true)
		this->direction = MOVE_DIRECTION::RIGHT;
	else
		this->direction = MOVE_DIRECTION::IDLE;

	if (Keyboard::isKeyPressed(Keyboard::J) == true)
		this->rotate_direction = ROTATE_DIRECTION::COUNTER_CLOCKWISE;
	else if (Keyboard::isKeyPressed(Keyboard::K) == true)
		this->rotate_direction = ROTATE_DIRECTION::CLOCKWISE;
	else
		this->rotate_direction = ROTATE_DIRECTION::IDLE;

    if (Keyboard::isKeyPressed(Keyboard::Space) == true && m_is_fire_button_down == false) {
        this->is_fire_active = true;
    }
    else {
        this->is_fire_active = false;
    }
    m_is_fire_button_down = Keyboard::isKeyPressed(Keyboard::Space);
}

void PlayerController::Start()
{
	// decide how you want to spend the 10 skill points
	// you can change the value, but the summary shouldn't larger than 10
	int engine_upgrade = 6;
	int armor_upgrade = 2;
	int turret_upgrade = 2;

	m_tank_ptr->UpgradeTank(engine_upgrade, armor_upgrade, turret_upgrade);
}