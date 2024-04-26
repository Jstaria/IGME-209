#pragma once
#include "AIController.h"
#include <vector>

class Tank;

class MyController :
    public AIController
{
public:
    MyController(Tank* tank) : AIController(tank) {};
    MyController() :AIController() {};
    void Start() override;

private:
    void MakeADecision() override;

    // define your own variables below
    int distance_to_map_center[80][80]; // center as 40, 40
    int distance_to_tank[80][80]; // range as 21
    // define your own variables above

    void Path_Scanner(Vector2u source, int result[80][80], int limit);
    float GetRotationByTarget(Vector2f position_tank, Vector2f position_target);
};

