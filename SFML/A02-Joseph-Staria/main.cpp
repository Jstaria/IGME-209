#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tank.h"     
#include "TankMap.h"  
#include "TankController.h"
#include "PlayerController.h"
#include "AIController.h"
#include "MyController.h"
#include "ViewManager.h"
#include "UpgradeItem.h"
#include "InformationYouCanGet.h"
#include <fstream>

using namespace std;

ViewManager* g_viewManager; // view managere

TankMap* g_map_ptr;     // map pointer

vector<Tank*> g_tank_array;

// deltaTime
Time g_preTime;         // the ElapsedTime of last frame
Clock g_deltaClock;     // the clock used to get the ElapsedTime

BulletManager* g_bullet_manager; // manage all bullets
UpgradeItemManager* g_item_manager; // manage all items

// used to random start position
vector<AIController*> controllers;
vector<Vector2u> g_start_positions;

bool is_pause_down = false;
bool is_pause = false;

bool is_speed_up_down = false;
bool is_speed_down_down = false;
float speedScalers[] = { 0.5f, 1.0f, 2.0f};
int speedlevelNum = 3;
int currentScalerIndex = 1;

#pragma region Initialization

Vector2u GetARandomStartPositionIndex() {
    if (g_start_positions.size() == 0)
        return Vector2u(0, 0);
    else {
        int n = rand() % g_start_positions.size();
        Vector2u v = g_start_positions[n];
        g_start_positions.erase(g_start_positions.begin() + n);
        return v;
    }
}

void InitTank(sf::RenderWindow& window) {
    for (int i = 0; i < 30; i++) {
        controllers.push_back(new MyController());
    }

    for (int i = 0; i < controllers.size(); i++) {
        Vector2f position = g_map_ptr->GetCenterPositionByIndex(GetARandomStartPositionIndex());

        Tank* tank_ptr = new Tank(Color::White, position, i + 1, "Player" + to_string(i + 1));

        controllers[i]->SetTank(tank_ptr);
        controllers[i]->Start();
        g_tank_array.push_back(tank_ptr);
    } 
}

void InitMap(sf::RenderWindow& window) {
    // create and load the game map
    g_map_ptr = new TankMap("Maps/level1.txt");

    g_item_manager = new UpgradeItemManager();

    for (int i = 0; i < g_map_ptr->GetMapSize().x; i++) {
        for (int j = 0; j < g_map_ptr->GetMapSize().y; j++) {
            if (g_map_ptr->GetTerrainByIndex(i, j).name == "dirt3") {
                g_start_positions.push_back(Vector2u(i, j));
            }
        }
    }
}
void InitClock(sf::RenderWindow& window) {
    // let the clock start to clocking the time 
    g_deltaClock.restart();
    g_preTime = g_deltaClock.getElapsedTime(); // set the default value of previous time
}

void InitView(RenderWindow& window) {
    g_viewManager = new ViewManager(window, g_map_ptr, g_tank_array);
}

// initialization
void Init(sf::RenderWindow& window) {
    string str = "IGM209-03";

    ifstream inf;
    inf.open("Assets/seed.txt"); // open the file
    inf >> str;
    inf.close();    // close the file

    int seed = 0;
    for (int i = 0; i < str.size(); i++)
        seed += (int) str[i];
    srand(seed); // set a seed
    InitMap(window);
    InitTank(window);
    InitView(window);
    InitClock(window);

    g_bullet_manager = new BulletManager();
}
#pragma endregion

#pragma region Update
float UpdateDeltaTime() {
    Time currentTime = g_deltaClock.getElapsedTime();   // get the current time
    Time deltaTime = currentTime - g_preTime;           // calculate the delta time
    float deltaSeconds = deltaTime.asSeconds();         // time to seconds
    g_preTime = currentTime;                            // the current time become the preTime for next frame

    return deltaSeconds * speedScalers[currentScalerIndex];
}
// update game logic
void Update(sf::RenderWindow& window) {
    float deltaSeconds = UpdateDeltaTime();

    if (is_pause == false) {

        for (int i = 0; i < g_tank_array.size(); i++) {
            ((AIController*)g_tank_array[i]->GetController())->UpdateAllInformation(g_tank_array, g_item_manager->item_array, g_bullet_manager->bullet_array);
            g_tank_array[i]->Update(window, deltaSeconds);
        }

        g_bullet_manager->Update(window, deltaSeconds);

        g_map_ptr->Update(window, deltaSeconds);
        g_item_manager->Update(window, deltaSeconds);



        // check bullet
        for (int i = 0; i < g_bullet_manager->bullet_array.size(); i++) {
            Bullet* bullet = g_bullet_manager->bullet_array[i];

            for (int j = 0; j < g_tank_array.size(); j++) {
                g_tank_array[j]->CheckGetHit(bullet);
            }
        }

        // eat item
        for (int i = 0; i < g_item_manager->item_array.size(); i++) {
            UpgradeItem* item = g_item_manager->item_array[i];

            for (int j = 0; j < g_tank_array.size(); j++) {
                g_tank_array[j]->CheckEatItem(item);
            }
        }
    }
    g_viewManager->Update(window, deltaSeconds);

    if (is_pause_down == false && Keyboard::isKeyPressed(Keyboard::P)) {
        is_pause = !is_pause;
    }

    is_pause_down = Keyboard::isKeyPressed(Keyboard::P);


    if (is_speed_up_down == false && Keyboard::isKeyPressed(Keyboard::RBracket)) {
        currentScalerIndex = (currentScalerIndex + 1);
        if (currentScalerIndex >= speedlevelNum)
            currentScalerIndex = currentScalerIndex - 1;
    }
    is_speed_up_down = Keyboard::isKeyPressed(Keyboard::RBracket);

    if (is_speed_down_down == false && Keyboard::isKeyPressed(Keyboard::LBracket)) {
        currentScalerIndex = (currentScalerIndex - 1);
        if (currentScalerIndex < 0)
            currentScalerIndex = 0;
    }
    is_speed_down_down = Keyboard::isKeyPressed(Keyboard::LBracket);
}
#pragma endregion

// render the game scene
void Render(sf::RenderWindow& window) {
    g_map_ptr->Render(window);      // draw the map

    // draw tanks
    for (int i = 0; i < g_tank_array.size(); i++)
        g_tank_array[i]->Render(window);

    g_item_manager->Render(window);
    g_bullet_manager->Render(window);

    g_viewManager->Render(window); // last
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(960, 960), "IGME 209 - Tank Campionship"); // create a window
    window.setFramerateLimit(60);

    Init(window); // initialization

    // game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(0, 0, 0)); // clear the window with color(0,0,0)

        Update(window); // update game logic
        Render(window); // draw the game scene

        window.display(); // display the window
    }

    return 0;
}