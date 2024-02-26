#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

const int ROWS = 20;
const int COLS = 20;
const int BLOCK_SIZE = 50; // Size of each block in pixels
char maze[ROWS][COLS] = {
	// Maze initialization remains the same
	{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	{'#', 'P', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#'},
	{'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#'},
	{'#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', '#'},
	{'#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#', ' ', '#', 'X', '#'},
	{'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', ' ', '#', ' ', '#'},
	{'#', ' ', '#', 'X', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#', ' ', '#'},
	{'#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', ' ', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#'},
	{'#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', ' ', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', '#', '#'},
	{'#', ' ', '#', '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', '#', ' ', '#', ' ', '#', '#'},
	{'#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', '#'},
	{'#', ' ', ' ', 'X', ' ', '#', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
	{'#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
	{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'},
	{'#', 'E', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

int playerX = 1;
int playerY = 1;
bool isGameOver = false;

void drawRect(Color color, sf::RenderWindow& window, RectangleShape rectangle, int i, int j) {
	rectangle.setFillColor(color); // Set the rectangle's color
	rectangle.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE); // Set the position of the rectangle
	window.draw(rectangle); // Draw the rectangle
}

// Function to draw the maze
void drawMaze(sf::RenderWindow& window) {
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			RectangleShape rectangle(Vector2f(BLOCK_SIZE, BLOCK_SIZE)); // Size of rectangle

			Color color = Color::White;

			switch (maze[i][j])
			{
				case '#':
					color = Color::Black;

					drawRect(color, window, rectangle, i, j);

					break;

				case ' ':
					color = Color::White;

					drawRect(color, window, rectangle, i, j);

					break;

				case 'E':
					color = Color::Blue;

					drawRect(color, window, rectangle, i, j);

					break;

				case 'X':
					color = Color::Red;

					drawRect(color, window, rectangle, i, j);

					break;

				case 'P':

					drawRect(color, window, rectangle, i, j);

					color = Color::Green;

					RectangleShape square(Vector2f(10,20));
					square.setFillColor(color);
					square.setPosition(j * BLOCK_SIZE + (BLOCK_SIZE / 2) - 5, i * BLOCK_SIZE + 30);
					window.draw(square);

					color = Color::Yellow;

					int radius = 10;

					CircleShape circle(radius);
					circle.setFillColor(color);
					circle.setPosition(j * BLOCK_SIZE + (BLOCK_SIZE / 2) - radius, i * BLOCK_SIZE + (BLOCK_SIZE / 2) - radius);
					window.draw(circle);

					break;
			}
		}
	}
}

// Updated movePlayer function with SFML logic here
bool movePlayer(char input) {
	int newX = 0;
	int newY = 0;

	switch (input) {
	case 'w':

		newX = playerX - 1;
		newY = playerY;
		break;

	case 'a':

		newX = playerX;
		newY = playerY - 1;
		break;

	case 's':

		newX = playerX + 1;
		newY = playerY;
		break;

	case 'd':

		newX = playerX;
		newY = playerY + 1;
		break;
	}

	// 2. Check if the new position is within the bounds of the maze and not blocked by a wall ('#').
	//    - If the move is invalid (out of bounds or into a wall), do not update the player's position.

	if (newX < 0 || newY < 0 || newX >= ROWS || newY >= COLS || maze[newX][newY] == '#') return false;

	// 3. Update the player's position on the maze map:
	//    - Clear the current position of the player by setting it to ' ' (space).
	//    - Mark the new position with 'P' to represent the player.

	char v = maze[newX][newY];

	maze[playerX][playerY] = ' ';
	maze[newX][newY] = 'P';

	playerX = newX;
	playerY = newY;

	// 4. Check the new position for an enemy ('X') or the exit ('E'):
	//    - If the player reaches the enemy ('X'), display a message indicating the game is over due to encountering an enemy and set `isGameOver` to true.
	//    - If the player reaches the exit ('E'), display a congratulatory message indicating the player has escaped the maze and set `isGameOver` to true.
	// Note: The game's main loop checks the `isGameOver` flag to determine if the game should end.

	if (v == 'X') {
		printf("You've run into an enemy! GAME OVER\n");
		isGameOver = true;
	}
	else if (v == 'E') {
		printf("You've escaped! GAME WON\n");
		isGameOver = true;
	}


	return true;
	// End your implementation above this comment  
}

int main() {
	sf::RenderWindow window(sf::VideoMode(COLS * BLOCK_SIZE, ROWS * BLOCK_SIZE), "Maze Game");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && !isGameOver) {
				// Handle player movement
				if (event.key.code == sf::Keyboard::W) movePlayer('w');
				else if (event.key.code == sf::Keyboard::S) movePlayer('s');
				else if (event.key.code == sf::Keyboard::A) movePlayer('a');
				else if (event.key.code == sf::Keyboard::D) movePlayer('d');
			}
		}

		window.clear();
		drawMaze(window);
		window.display();

		if (isGameOver) {
			// Handle game over state
			// For simplicity, we'll just close the window
			window.close();
		}
	}

	system("pause");

	return 0;
}
