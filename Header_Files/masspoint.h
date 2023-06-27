#pragma once
#include "../Header_Files/defines.h"

class Masspoint
{
public:
	Masspoint(vec2 pos, float mass, float radius);
	Masspoint();

	void draw(sf::RenderWindow* window);
	void addForce(vec2 f);
	void physicsStep(float delta);
	void reset();

	vec2 position;
	vec2 velocity;
	vec2 force;
	float mass;
	float radius;

	static int nextID;
	int ID = 0;

	sf::CircleShape dot;

	
};