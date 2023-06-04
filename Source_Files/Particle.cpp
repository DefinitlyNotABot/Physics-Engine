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


	positionSave = position;
	


	center = position + vec2(radius, radius);

	if (center.y > SCREEN_HEIGHT - radius) {
		moveDir.y *= -1;
		moveDir.y *= bouncyness;
		position.y = 600 - 2 * radius;
	}

	if (center.x < 0 + radius) {
		moveDir.x *= -1;
		moveDir.x *= bouncyness;
		position.x = 0 + 2 * radius;
	}
	else if (center.x > SCREEN_WIDTH - radius) {
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
		vec2 mp1 = center;
		vec2 mp2 = p.center;

		float sz = ((float)(mp2.y - mp1.y) / (float)(mp2.x - mp1.x));
		if (sz == 0)
		{
			sz = std::numeric_limits<float>::min();
		}
		float st = (float)-1 / sz;
		if (st == 0)
		{
			st = std::numeric_limits<float>::min();
		}

		vec2 move1 = moveDir;

		float sv1 = ((move1.y) / (move1.x));
		
		float xt1 = move1.x * ((sz - sv1) / (sz - st));
		float xz1 = move1.x * ((st - sv1) / (sz - st));
		float yt1 = xt1 * st;
		float yz1 = xz1 * sz;

		vec2 vt1 = vec2(xt1, yt1);
		vec2 vz1 = vec2(xz1, yz1);

		vec2 nv1 = vt1 + vz1;

		moveDirSave = nv1;

		vec2 mp12 = (mp1 - mp2) / 2.0f;

		positionSave = position + mp12;
	}
}

void Particle::collisionUpdatePos()
{
	position = positionSave;
	moveDir = moveDirSave;
}

