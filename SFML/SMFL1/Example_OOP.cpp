#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Base class for all animals. It contains common attributes and methods that all animals share.
class Animal {
protected:
    sf::Texture texture;
    sf::Sprite sprite;

    bool loadTexture(const std::string& filePath) {
        if (!texture.loadFromFile(filePath)) {
            std::cerr << "Error loading texture from: " << filePath << std::endl;
            return false;
        }
        sprite.setTexture(texture);
        return true;
    }

public:
    Animal() = default;             // Default constructor
    virtual ~Animal() = default;    // Virtual destructor for proper cleanup in derived classes

    // Draws the animal's sprite at the given position. This method could be overridden if needed.
    virtual void draw(sf::RenderWindow& window, float x, float y) {
        sprite.setPosition(x, y);
        window.draw(sprite);
    }

    // Pure virtual method that derived classes must implement to produce their unique sound.
    virtual void sing() const = 0;
};

// Derived class for Chicken. It inherits from Animal and specifies its unique attributes and behaviors.
class Chicken : public Animal {
public:
    Chicken() {
        loadTexture("Images/chicken.png"); // Load chicken-specific texture
    }

    // Implements the sing method for Chicken
    void sing() const override {
        std::cout << "Cluck!" << std::endl;
    }
};

// Similar derived classes for Duck, Dog, and Cat...
class Duck : public Animal {
public:
    Duck() {
        loadTexture("Images/duck.png");
    }

    void sing() const override {
        std::cout << "Quack!" << std::endl;
    }
};


class Dog : public Animal {
public:
    Dog() {
        loadTexture("Images/dog.png");
    }

    void sing() const override {
        std::cout << "Woof!" << std::endl;
    }
};

class Cat : public Animal {
public:
    Cat() {
        loadTexture("Images/cat.png");
    }

    void sing() const override {
        std::cout << "Meow!" << std::endl;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML OOP Example");

    std::vector<Animal*> animals;
    animals.push_back(new Cat());
    animals.push_back(new Dog());
    animals.push_back(new Chicken());
    animals.push_back(new Duck());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(255, 255, 255));

        // Polymophsim
        for (int i = 0; i < animals.size(); i++) {
            animals[i]->draw(window, i * 150 + 100, 200);
            animals[i]->sing();
        }
        window.display();
    }

    return 0;
}