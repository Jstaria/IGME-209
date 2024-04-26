#include "MyController.h"
#include <queue>

void MyController::Start() 
{
	information = new InformationYouCanGet(m_tank_ptr);

	/***********************************************/
	// decide how you want to spend the 10 skill points to upgrade your tank
	// You can change the value below
	// the summary needs to equal to 10
	unsigned int engine_upgrade = 0;
	unsigned int armor_upgrade = 0;
	unsigned int turret_upgrade = 10;

	// give your tank a nickname
	// you can change the value below, don't be too long
	// you can set the name as "anonymous" if you want.
	name_of_my_tank = "Trigger Happy";

	// initialize the variable you defined below
	// if you don't define any new variable in the MyControler.h, you can leave it blank

	// initialize the variable you defined above

	/**********************************************/

	Path_Scanner(Vector2u(40, 40), distance_to_map_center, 1000); // path to the center

	m_tank_ptr->SetName(name_of_my_tank);
	m_tank_ptr->UpgradeTank(engine_upgrade, armor_upgrade, turret_upgrade); // upgrade your tank
	MakeADecision(); // make the first decision
}

void MyController::MakeADecision()
{
	/*****************************************/
	// Write your code below
	
	// Here is an example of how to get information from the information object
	Vector2f position = information->my_tank_position(); // get the current position of your tank, for example (250, 350)
	Vector2u block_index = information->PositionToMapIndex(position); // get the currect block index of your tank, for example [5][6]

	// Here is an example of how to get the distance to all the blocks
	Vector2u tank_map_index = information->my_tank_map_index();
	this->Path_Scanner(tank_map_index, distance_to_tank, 20);	// limited the scope
	distance_to_tank[tank_map_index.x][tank_map_index.y];

	// How to use score to measure the block and choose the best one
	int score[80][80];
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			score[i][j] = 0;
		}
	}
	
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			int distance = distance_to_map_center[i][j]; // consider distance to the center
			if (distance == -1)
				score[i][j] -= 1000;
			else
				score[i][j] -= distance;
		}
	}

	// find the highest score as destination
	Vector2u target_map_index = Vector2u(40, 40);
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			if (score[i][j] > score[target_map_index.x][target_map_index.y])
				target_map_index = Vector2u(i, j);
		}
	}

	vector<Vector2f> nearby = information->nearby_enenmy_tank_position();
	
	if (nearby.size() > 0) {
		Vector2f closest = nearby[0];
		target_map_index = information->PositionToMapIndex(closest);
	}


	// Here is an example of how to set a destination
	this->SetDestination(Vector2u(target_map_index.x, target_map_index.y));

	

	// Here is an example of how to set a rotation
	int rotation = rand() % 360; // generate a new random rotation target
	this->SetTurretRotation(rotation);

	// Write your code above
	/*****************************************/

	is_fire_active = true; // always fire
}

void MyController::Path_Scanner(Vector2u source, int result[80][80], int limit)
{
	int const offset_x[4] = { -1, 1, 0, 0 }; // up down left right
	int const offset_y[4] = { 0, 0, -1, 1 }; // up down left right

	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			result[i][j] = -1; // set the initial distance as -1, which means is not walkable
		}
	}
	result[source.x][source.y] = 0; // set the distance from source to itself = 0

	// write your code blow
	queue<Vector2u> m_queue;
	bool visited[80][80] = { false };
	m_queue.push(source);
	visited[source.x][source.y] = true;

	while (m_queue.empty() == false) {
		Vector2u node = m_queue.front();
		m_queue.pop();

		if (result[node.x][node.y] > limit) // set a maxium step
			return;

		for (int k = 0; k < 4; k++) {
			Vector2u nextNode = Vector2u(node.x + offset_x[k], node.y + offset_y[k]);

			if (nextNode.x >= 0 && nextNode.y >= 0 && nextNode.x < 80 && nextNode.y < 80 && visited[nextNode.x][nextNode.y] == false) {
				if (information->GetTerrain_from_MapIndex(nextNode).tank_passable == true) {
					visited[nextNode.x][nextNode.y] = true;
					m_queue.push(nextNode);
					result[nextNode.x][nextNode.y] = result[node.x][node.y] + 1; // the distance of nextNode = the distance of node + 1 
				}
			}
		}
	}
}