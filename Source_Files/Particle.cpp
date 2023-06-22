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

void Particle::draw(sf::RenderWindow* window)
{
	window->draw(dot);
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
	// Calculate the distance between the centers of the two particles
	vec2 delta = p.center - center;
	float distanceSq = glm::dot(delta, delta);

	// Check if the particles are colliding
	float radiusSum = radius + p.radius;
	float radiusSumSq = radiusSum * radiusSum;
	if (distanceSq <= radiusSumSq)
	{
		// Calculate the collision normal and unit vector
		vec2 collisionNormal = normalize(delta);

		// Calculate the relative velocity
		vec2 relativeVelocity = p.moveDir - moveDir;

		// Calculate the impulse scalar
		float impulseScalar = glm::dot(relativeVelocity, collisionNormal) * (1 + bouncyness) / (1 / mass + 1 / p.mass);

		// Apply impulses to update the velocities
		moveDirSave += impulseScalar * collisionNormal / mass;
		

		// Separate the particles to avoid overlap
		vec2 separation = collisionNormal * (radiusSum - sqrt(distanceSq)) * 0.5f;
		positionSave -= separation;
		
	}
}

void Particle::collisionUpdatePos()
{
	position = positionSave;
	moveDir = moveDirSave;
}

