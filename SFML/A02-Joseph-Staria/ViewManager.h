#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "TankMap.h"
#include "Tank.h"
#include "stack"

using namespace std;
using namespace sf;

class ViewManager
{
public:
	static ViewManager* s_instance;
private:
	View m_free_view;
	View m_focus_view;

	bool m_is_full_camera_button_down = false;
	bool m_is_next_button_down = false;

	enum class VIEW_TYPE {
		FOCUS = 0,
		FREE = 1
	};

	VIEW_TYPE m_view_type = VIEW_TYPE::FOCUS;
	vector<Tank*> m_tank_arr;

	Font font;

	float m_scaler = 1.0f;

	int m_curret_focus = 0;

	vector<string> kill_message;

	int m_tank_left;

	bool is_first_blood = false;
	// Last Surviver
	string last_surviver;
	// Last Surviver Runner-Up
	vector<string> last_surviver_runner_up;
	// First Blood
	string first_blood;
	// Master of Destruction
	int master_of_destruction_num = 0;
	vector<string> master_of_destruction;
	// Tank Among Tanks
	int tank_among_tanks_num = 0;
	vector<string> tank_among_tanks;
	// Best Engineer
	vector<string> best_engineer;

public:
	ViewManager(RenderWindow& window, TankMap* tankMap_ptr, vector<Tank*> tank_arr);
	void Update(RenderWindow& window, float deltaSeconds);
	void Render(RenderWindow& window);
	View GetFullView();

	void AddKill(string killer, string target);
};

