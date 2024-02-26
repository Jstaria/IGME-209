#include <SFML/Graphics.hpp>

using namespace sf;

void drawKirby(RenderWindow& window)
{
    RectangleShape rect1(Vector2f(50, 70.f)); // Size of rectangle
    rect1.setFillColor(Color::Red); // Set the rectangle's color
    rect1.setPosition(230, 300); // Set the position of the rectangle
    rect1.setRotation(45);

    window.draw(rect1); // Draw the rectangle

    RectangleShape rect(Vector2f(50, 70.f)); // Size of rectangle
    rect.setFillColor(Color::Red); // Set the rectangle's color
    rect.setPosition(270, 330); // Set the position of the rectangle
    rect.setRotation(-45);

    window.draw(rect); // Draw the rectangle

    // Create a rectangle shape
    RectangleShape rectangle(Vector2f(120.f, 120.f)); // Size of rectangle
    rectangle.setFillColor(Color::Magenta); // Set the rectangle's color
    rectangle.setPosition(200, 200); // Set the position of the rectangle

    window.draw(rectangle); // Draw the rectangle

    RectangleShape rect2(Vector2f(12.f, 120.f)); // Size of rectangle
    rect2.setFillColor(Color::Magenta); // Set the rectangle's color
    rect2.setPosition(250, 200); // Set the position of the rectangle
    rect2.setRotation(45);

    window.draw(rect2); // Draw the rectangle

    RectangleShape rect3(Vector2f(12.f, 120.f)); // Size of rectangle
    rect3.setFillColor(Color::Magenta); // Set the rectangle's color
    rect3.setPosition(350, 200); // Set the position of the rectangle
    rect3.setRotation(45);

    window.draw(rect3); // Draw the rectangle

    CircleShape circ1(20);

    circ1.setFillColor(Color::Black);
    circ1.setPosition(230, 230);
    circ1.setScale(Vector2f(.5f, 1));

    window.draw(circ1);

    circ1.setPosition(280, 230);

    window.draw(circ1);
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
        drawKirby(window); // Call the separate function to draw the rectangle
        window.display(); // Update the window with what we've drawn
    }

    return 0;
}