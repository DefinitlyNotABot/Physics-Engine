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

	t0 = Time::now();
	
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


void Particle::physicsStep()
{
	if (!step_calculated)
	{
		t1 = Time::now();
		fsec fs = t1 - t0;
		float delta = fs.count();
		this->delta = delta;

		// Calculate Gravity
		moveDir.y += g * delta;
		// ~Calculate Gravity

		// Calculate Forces
		float forceLength = glm::length(force);
		if (forceLength > 0)
		{
			float a = forceLength / mass;
			moveDir += glm::normalize(force) * a * delta;
		}

		position += moveDir;
		speed = glm::length(moveDir);

		center = position + vec2(radius, radius);

		if (center.y > SCREEN_HEIGHT - radius)
		{
			position.y = SCREEN_HEIGHT - 2 * radius;
			moveDir.y *= -bouncyness;
		}

		if (center.x < radius)
		{
			moveDir.x *= -bouncyness;
			position.x = 2 * radius;
		}
		else if (center.x > SCREEN_WIDTH - radius)
		{
			moveDir.x *= -bouncyness;
			position.x = SCREEN_WIDTH - 2 * radius;
		}

		moveDirSave = moveDir;
		dot.setPosition(position.x, position.y); // Move Dot to new Position
		positionSave = position;
		t0 = Time::now();
	}
	
}

void Particle::collision(const Particle& p)
{
	float dx = p.center.x - center.x;
	float dy = p.center.y - center.y;
	float radiusSum = radius + p.radius;
	float radiusSumSq = radiusSum * radiusSum;

	float distanceSq = dx * dx + dy * dy;

	if (distanceSq <= radiusSumSq)
	{
		vec2 mp1 = center;
		vec2 mp2 = p.center;

		vec2 move1 = moveDir;
		float dx = mp2.x - mp1.x;
		float dy = mp2.y - mp1.y;
		float radiusSum = radius + p.radius;

		float invMag = 1.0f / sqrt(dx * dx + dy * dy);
		float nx = dx * invMag;
		float ny = dy * invMag;

		float dotProduct = move1.x * nx + move1.y * ny;
		float px = nx * dotProduct;
		float py = ny * dotProduct;

		vec2 vt1 = vec2(px, py);
		vec2 vz1 = move1 - vt1;
		vec2 nv1 = vt1 - vz1;

		moveDirSave = nv1;

		vec2 mp12 = (mp1 - mp2) * 0.5f;
		positionSave = position + mp12;
	}
}

void Particle::collisionUpdatePos()
{
	position = positionSave;
	moveDir = moveDirSave;
}

