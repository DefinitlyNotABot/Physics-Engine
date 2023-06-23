#pragma once
#include "../Header_Files/defines.h"
#include "PhysicsObject.h"


class Triangle : public PhysicsObject
{
public:
	Triangle(vec2 pos, double rad, sfCol col, bool hollow, float bouncyness, float m);
	void draw(sf::RenderWindow& window) override;
	void physicsStep() override;

	void collision(const Particle& p);
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