#include "AIController.h"
#include <queue>


void AIController::Start()
{
	information = new InformationYouCanGet(m_tank_ptr);
	// decide how you want to spend the 10 skill points
	int engine_upgrade = 6;
	int armor_upgrade = 2;
	int turret_upgrade = 2;

	m_tank_ptr->UpgradeTank(engine_upgrade, armor_upgrade, turret_upgrade);

	MakeADecision();
}

void AIController::UpdateAllInformation(vector<Tank*> all_tank, vector<UpgradeItem*> all_item, vector<Bullet*> all_bullet)
{
	information->Update(all_tank, all_item, all_bullet);
}

void AIController::Update(RenderWindow& window, float deltaSeconds)
{
	m_time_to_next_decision -= deltaSeconds;
	if (m_time_to_next_decision <= 0.0f) {
		MakeADecision();
		m_time_to_next_decision = 0.8f + rand() % 40 / 100.0f;
	}

	float turret_rotation = m_tank_ptr->GetTurretRotatoin();
	float diff = m_turret_target_rotation - turret_rotation;

	if (abs(diff) < 1.0f)
		rotate_direction = ROTATE_DIRECTION::IDLE;
	else {
		if (diff < 0.0f)
			diff += 360.0f;
		if (diff < 180.0f)
			rotate_direction = ROTATE_DIRECTION::CLOCKWISE;
		else
			rotate_direction = ROTATE_DIRECTION::COUNTER_CLOCKWISE;
	}

	direction = MoveTowardDestination();
}

void AIController::MakeADecision()
{

}

#pragma region FunctionProvided

TankController::MOVE_DIRECTION AIController::MoveTowardDestination()
{
	Vector2f current_position = this->m_tank_ptr->GetPosition();

	bool arrive_destination = false;
	if (direction == MOVE_DIRECTION::LEFT && current_position.x <= m_next_destination.x) {
		arrive_destination = true;
	}
	else if (direction == MOVE_DIRECTION::RIGHT && current_position.x >= m_next_destination.x) {
		arrive_destination = true;
	}
	else if (direction == MOVE_DIRECTION::UP && current_position.y <= m_next_destination.y) {
		arrive_destination = true;
	}
	else if (direction == MOVE_DIRECTION::DOWN && current_position.y >= m_next_destination.y) {
		arrive_destination = true;
	}

	//if (distance(current_position, m_next_destination) < ERROR_RANGE) { // reach the center
	//	arrive_destination = true;
	//}

	if (arrive_destination == true) {
		Vector2u current_index = TankMap::s_instance->GetIndexByPosition(current_position); // current index
		int k = m_path[current_index.x][current_index.y]; // next direction

		if (k < 0) {
			return MOVE_DIRECTION::IDLE;
		}

		Vector2u next_index = current_index + Vector2u(offset_x[k], offset_y[k]); // next index
		Vector2f next_position = TankMap::s_instance->GetCenterPositionByIndex(next_index); // next center position;
		m_next_destination = next_position;

		return (MOVE_DIRECTION)k;
	}

	return direction;
}

float distance_vector2f(Vector2f v1, Vector2f v2) {
	float x = v1.x - v2.x;
	float y = v1.y - v2.y;
	return sqrt(x * x + y * y);
}

bool AIController::SetDestination(Vector2u destination)
{
	if (destination.x == m_final_destination.x && destination.y == m_final_destination.y)
		return true;
	for (int i = 0; i < MAX_MAP_SIZE; i++) {
		for (int j = 0; j < MAX_MAP_SIZE; j++) {
			m_reverse_path[i][j] = -1;
			m_path[i][j] = -1;
		}
	}

	Vector2u source = TankMap::s_instance->GetIndexByPosition(this->m_tank_ptr->GetPosition());

	bool path_existing = PathFindingBFS(source, destination);
	if (path_existing == true) {
		m_final_destination = TankMap::s_instance->GetCenterPositionByIndex(destination);


		m_next_destination = TankMap::s_instance->GetCenterPositionByIndex(source);

		// the tank needs to go to the center of the current block first before moving towards the target
		Vector2f tank_position = m_tank_ptr->GetPosition(); // tank position
		int k = m_path[source.x][source.y];

		if (distance_vector2f(m_next_destination, m_tank_ptr->GetPosition()) < 5.0f) {
			Vector2u current_index = TankMap::s_instance->GetIndexByPosition(this->m_tank_ptr->GetPosition()); // current index
			int k = m_path[current_index.x][current_index.y]; // next direction

			if (k < 0) {
				direction = MOVE_DIRECTION::IDLE;
			}

			Vector2u next_index = current_index + Vector2u(offset_x[k], offset_y[k]); // next index
			Vector2f next_position = TankMap::s_instance->GetCenterPositionByIndex(next_index); // next center position;
			m_next_destination = next_position;

			direction = (MOVE_DIRECTION)k;
		}
		else {
			if (k == 0 || k == 1) { // up down
				if (tank_position.x > m_next_destination.x)
					direction = MOVE_DIRECTION::LEFT; // tank at the right side of the block
				else
					direction = MOVE_DIRECTION::RIGHT; // tank at the left side of the block
			}
			else if (k == 2 || k == 3) { // left right
				if (tank_position.y < m_next_destination.y)
					direction = MOVE_DIRECTION::DOWN; // tank is above the block
				else
					direction = MOVE_DIRECTION::UP; // tank is under the block
			}
			else {
				direction = MOVE_DIRECTION::IDLE;
			}
		}
	}
	else {
		direction = MOVE_DIRECTION::IDLE;
	}
	return path_existing;
}

void AIController::SetTurretRotation(float rotation)
{
	while (rotation < 0.0f)
		rotation += 360.0f;
	while (rotation > 360.0f)
		rotation -= 360.0f;
	m_turret_target_rotation = rotation;
}

bool AIController::PathFindingBFS(Vector2u source, Vector2u destination) {
	queue<Vector2u> m_queue;
	m_queue.push(source);  // push the start point into the queue

	bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE] {false}; // the node has been visited
	visited[source.x][source.y] = true;

	while (m_queue.empty() == false) {
		Vector2u nodeCurrent = m_queue.front(); // get the first node of the queue
		m_queue.pop();

		if (nodeCurrent.x == destination.x && nodeCurrent.y == destination.y) { // found the destination
			// cout << "Path found :";
			PrintPath(source, destination);
			// cout << endl;
			return true;
		}

		// push all adjacent nodes into the queue if not visited
		int offset = rand() % 100;
		for (int i = 0; i < 4; i++) {
			int k = (i + offset) % 4; // random change the order of the 4 adjacent nodes
			Vector2u nodeNext = Vector2u(nodeCurrent.x + offset_x[k], nodeCurrent.y + offset_y[k]);

			// solution 1: calculate if the tank can go to the next node in real time
			if (TankMap::s_instance->GetTerrainByIndex(nodeNext.x, nodeNext.y).tank_passable == true 
				&& visited[nodeNext.x][nodeNext.y] == false) {
				m_queue.push(nodeNext);
				visited[nodeNext.x][nodeNext.y] = true;
				m_reverse_path[nodeNext.x][nodeNext.y] = k;
			}
		}
	}

	// cout << "No path found" << endl;
	return false;
}

void AIController::PrintPath(Vector2u source, Vector2u destination)
{
	if (source.x != destination.x || source.y != destination.y) {
		int k = m_reverse_path[destination.x][destination.y];
		Vector2u one_step_back = Vector2u(destination.x - offset_x[k], destination.y - offset_y[k]);
		m_path[one_step_back.x][one_step_back.y] = k;

		PrintPath(source, one_step_back);
	}
	// cout << "(" << destination.x << "," << destination.y << ") ";
}

#pragma endregion

