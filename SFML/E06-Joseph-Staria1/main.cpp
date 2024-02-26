#include <SFML/Graphics.hpp>

using namespace sf;

void drawRectangle(RenderWindow& window)
{
    // Create a rectangle shape
    RectangleShape rectangle(Vector2f(120.f, 50.f)); // Size of rectangle
    rectangle.setFillColor(Color::Blue); // Set the rectangle's color
    rectangle.setPosition(200, 300); // Set the position of the rectangle

    window.draw(rectangle); // Draw the rectangle
}

int main()
{
    RenderWindow window(VideoMode(800, 600), "My window");

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        // Check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            // "Close requested" event: we close the window
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(); // Clear the window with the background color
        drawRectangle(window); // Call the separate function to draw the rectangle
        window.display(); // Update the window with what we've drawn
    }

    return 0;
}