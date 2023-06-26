#include <iostream>

#include "../Header_Files/PhysicsObject.h"

int PhysicsObject::nextID = 0;

PhysicsObject::PhysicsObject()
{
	rotation = 0.0f;
	rotationSpeed = 0.0f;

	force = vec2(0, 0);
	moveDir = vec2(0, 0);
	type = PH_OBJ;
}

void PhysicsObject::reset()
{
	force = vec2(0, 0);
	step_calculated = false;
	interacted_objects.clear();
}

void PhysicsObject::addForce(vec2* dir)
{
	force += *dir;
}

void PhysicsObject::draw(sf::RenderWindow* window)
{
	std::cout << "Draw not overridden" << std::endl;
}

void PhysicsObject::physicsStep(int chunk_id)
{
	std::cout << "PhysicsStep not overridden" << std::endl;
}


void PhysicsObject::collision(PhysicsObject& p)
{
	std::cout << "Collision not overridden" << std::endl;
}

void PhysicsObject::collisionUpdatePos()
{
	std::cout << "collisionUpdatePos not overridden" << std::endl;
}

void PhysicsObject::updateEnergy()
{
	energy_p = mass * g * ((SCREEN_HEIGHT - center.y)/10000); // E potential = m * g * h
	energy_k = 0.5 * mass * pow(glm::length(moveDir), 2); // E kinetic = 1/2 m * v²
	energy_all = energy_k + energy_p; 
}



