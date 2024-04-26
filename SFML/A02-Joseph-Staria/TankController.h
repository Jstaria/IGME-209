#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TankMap.h"
#include "Tank.h"
#include "InformationYouCanGet.h"

using namespace std;
using namespace sf;

class Tank;

class TankController {
public:
	enum class MOVE_DIRECTION {
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		IDLE = 4,
	};

	enum class ROTATE_DIRECTION {
		CLOCKWISE = 0,
		COUNTER_CLOCKWISE = 1,
		IDLE = 2
	};

public:
	/**************
	varaibles you need to change in the update function
	to control the tank
	***************/

	// fire or not
	bool is_fire_active = false;

	// move direction
	MOVE_DIRECTION direction = MOVE_DIRECTION::IDLE;

	//  // rotate direction
	ROTATE_DIRECTION rotate_direction = ROTATE_DIRECTION::IDLE;

public:
	TankController();
	TankController(Tank* tank);
	virtual void Update(RenderWindow& window, float deltaSeconds) = 0;
	void SetTank(Tank* tank);
	virtual void Start() = 0;


public:
	string name_of_my_tank = "Player";

protected:
	Tank* m_tank_ptr;
};