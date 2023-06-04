#pragma once
#include "../Header_Files/defines.h"
#include "PhysicsObject.h"


class Particle : public PhysicsObject
{
public:
	Particle(vec2 pos, double rad, sfCol col, bool hollow, float bouncyness, float m);
	void draw(sf::RenderWindow& window) override;
	void physicsStep(double delta) override;

	void collision(Particle p);
	void collisionUpdatePos();



	float speed = 0.0f;
	vec2 position;
	vec2 center;

	vec2 positionSave;

	float bouncyness;

	float mass;
	
	float radius;
	sfCol color;
	bool hollow;

	sf::CircleShape dot;

	void createDot();
	
};