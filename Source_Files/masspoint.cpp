#include <iostream>
#include <cmath>

#include "../Header_Files/masspoint.h"

int Masspoint::nextID = 0;

Masspoint::Masspoint(vec2 pos, float m, float rad)
{
	position = pos;
	velocity = vec2(0, 0);
	force = vec2(0, 0);
	mass = m;
	radius = rad;

	ID = nextID;
	nextID++;

	dot = sf::CircleShape(radius);

}


void Masspoint::draw(sf::RenderWindow* window)
{
	dot.setPosition(position.x - radius, position.y - radius);
	window->draw(dot);
}

void Masspoint::addForce(vec2 f)
{
	force += f;
}

void Masspoint::physicsStep(float delta)

{


	force += vec2(0, g);

	velocity += (delta * force) / mass;
	position += delta * velocity;

	if (position.y > SCREEN_HEIGHT - 2 * radius)
	{
		position.y = SCREEN_HEIGHT - 2 * radius;
		velocity.y *= -1;
	}


}

void Masspoint::reset()
{
	force = vec2(0, 0);
}