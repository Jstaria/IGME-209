#include "TankController.h"

TankController::TankController()
{
}

TankController::TankController(Tank* tank)
{
	m_tank_ptr = tank; 
	tank->SetController(this);
}

void TankController::SetTank(Tank* tank)
{
	m_tank_ptr = tank;
	tank->SetController(this);
}
