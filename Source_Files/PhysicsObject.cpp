#include <iostream>

#include "../Header_Files/PhysicsObject.h"

int PhysicsObject::nextID = 0;

PhysicsObject::PhysicsObject()
{
	rotation = 0.0f;
	angMomentum = 0.0f;

	force = vec2(0, 0);
	moveDir = vec2(0, 0);
}

void PhysicsObject::reset()
{
	force = vec2(0, 0);
}

void PhysicsObject::addForce(vec2 dir)
{
	force += dir;
}

void PhysicsObject::draw(sf::RenderWindow& window)
{
	std::cout << "Draw not overridden" << std::endl;
}

void PhysicsObject::physicsStep(double delta)
{
	std::cout << "PhysicsStep not overridden" << std::endl;
}



