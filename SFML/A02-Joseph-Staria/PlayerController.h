#pragma once
#include "TankController.h"

class Tank;

class PlayerController :
    public TankController
{
public:
	PlayerController(Tank* tank) : TankController(tank) {};
    void Update(RenderWindow& window, float deltaSeconds) override;
	void Start() override;

private:
    bool m_is_fire_button_down = true;
};

