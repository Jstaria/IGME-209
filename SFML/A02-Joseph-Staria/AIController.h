#pragma once
#include "TankController.h"
#include "InformationYouCanGet.h"
#include <vector>

#define MAX_MAP_SIZE 100
#define ERROR_RANGE 10.0f

class Tank;
class InformationYouCanGet;

class AIController :
    public TankController
{
#pragma region PreDefined
public:
    AIController() : TankController(){};
    AIController(Tank* tank) : TankController(tank) {};
    void UpdateAllInformation(vector<Tank*> all_tank, vector<UpgradeItem*> all_item, vector<Bullet*> all_bullet);
    void Update(RenderWindow& window, float deltaSeconds) override;
    void Start() override;

protected:
    /**************
    informations you can use to help your AI make decision
    also provided some basic functions
    ***************/
    InformationYouCanGet* information;

    int const offset_x[4] = { -1, 1, 0, 0 }; // up down left right
    int const offset_y[4] = { 0, 0, -1, 1 }; // up down left right

    int m_reverse_path[MAX_MAP_SIZE][MAX_MAP_SIZE]{ -1 };
    int m_path[MAX_MAP_SIZE][MAX_MAP_SIZE]{ -1 };

    Vector2f m_next_destination;    // index of next destination
    Vector2f m_final_destination;   // index of current destination

    float m_turret_target_rotation = 0.0f;

    float m_time_to_next_decision = 0.0f;

protected:
    MOVE_DIRECTION MoveTowardDestination();
    bool SetDestination(Vector2u destination); // set a new destination, return true if there exsit a path, path will be saved in m_path
    void SetTurretRotation(float rotation);

    bool PathFindingBFS(Vector2u source, Vector2u destination);
    void PrintPath(Vector2u source, Vector2u destination);

protected:
    virtual void MakeADecision();
#pragma endregion

#pragma region MyRegion
private:
    // define your own variables here

    // define your own variables above
private:
    // define your own functions below
    
    // define your own functions above
#pragma endregion
};

