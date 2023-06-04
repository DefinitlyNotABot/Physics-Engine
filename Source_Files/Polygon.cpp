#include <iostream>

#include "../Header_Files/defines.h"


col bgCol = col(0, 0, 0);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics Engine", sf::Style::Titlebar | sf::Style::Close);

    if (!window.isOpen()) {
        std::cout << "Window not created" << std::endl;
        return -1;
    }
    int c = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(bgCol);

        // Drawing a triangle
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, vec2(50, 50));
        triangle.setPoint(1, vec2(150, 150));
        triangle.setPoint(2, vec2(50, 150));
        triangle.setFillColor(bgCol);
        triangle.setOutlineThickness(1);
        triangle.setOutlineColor(col(255, 255, 255));
        
        triangle.rotate(0.1f*c);
        window.draw(triangle);

        // Drawing a dot
        sf::CircleShape dot(1);
        dot.setPosition(200, 100);
        dot.setFillColor(col(255, 255, 255));
        dot.setOutlineThickness(1);
        dot.setOutlineColor(col(255, 255, 255));
        window.draw(dot);

        window.display();
        c++;
    }

    return 0;
}