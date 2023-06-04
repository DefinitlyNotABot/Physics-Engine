#include <iostream>
#include <cmath>;

#include "../Header_Files/Particle.h"
sfCol bgCol2 = sfCol(0, 0, 0);

Particle::Particle(vec2 pos, double rad, sfCol col, bool isHollow, float bouncy, float m)
{
	
	position = pos;
	radius = rad;
	color = col;
	hollow = isHollow;
	bouncyness = bouncy;
	createDot();

	mass = m;

	moveDir = vec2(0, 0);

	ID = nextID;
	nextID++;
	
}

void Particle::draw(sf::RenderWindow& window)
{
    window.draw(dot);
}

void Particle::createDot()
{
	dot = sf::CircleShape(radius);
	dot.setPosition(position.x, position.y);
	if (hollow) {
		dot.setFillColor(bgCol2);
	}
	else {
		dot.setFillColor(color);
	}

	dot.setOutlineThickness(1);
	dot.setOutlineColor(color);
}


void Particle::physicsStep(double delta)
{
	this->delta = delta;

	// Calculate Gravity
	moveDir += vec2(0, g) * (float)delta;
	// ~Calculate Gravity


	// Calculate Forces
	float a = glm::length(force);

	if (a > 0)
	{
		moveDir += glm::normalize(force) * a * (float)delta;
	}


	position += moveDir;
	speed = glm::length(moveDir);
	energy_k = 0.5f * mass * pow(speed, 2);
	energy_p = mass * g * position.y;

	positionSave = position;
	


	center = position + vec2(radius, radius);

	if (center.y > 600 - radius) {
		moveDir.y *= -1;
		moveDir.y *= bouncyness;
		position.y = 600 - 2 * radius;
	}

	if (center.x < 0 + radius) {
		moveDir.x *= -1;
		moveDir.x *= bouncyness;
		position.x = 0 + 2 * radius;
	}
	else if (center.x > 800 - radius) {
		moveDir.x *= -1;
		moveDir.x *= bouncyness;
		position.x = 800 - 2 * radius;
	}
	moveDirSave = moveDir;
	dot.setPosition(position.x, position.y);	// Move Dot to new Position
	
	
}

void Particle::collision(Particle p)
{
	//std::cout << "me " << center.x << std::endl;

	float d = glm::length(center - p.center);
	float md = radius + p.radius;
	if (d <= md)
	{


		

	}
}

void Particle::collisionUpdatePos()
{
	position = positionSave;
	moveDir = moveDirSave;
}

