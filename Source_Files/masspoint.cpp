#include <iostream>
#include <cmath>

#include "../Header_Files/masspoint.h"


Masspoint::Masspoint(vec2 pos, float m, float rad)
{
	position = pos;
	velocity = vec2(0, 0);
	force = vec2(0, 0);
	mass = m;
	radius = rad;

	dot = sf::CircleShape(radius);
	dot.setPosition(position.x, position.y);
}

Masspoint::Masspoint()
{
	

}

void Masspoint::draw(sf::RenderWindow* window)
{
	window->draw(dot);
}