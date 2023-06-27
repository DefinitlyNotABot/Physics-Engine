#pragma once
#include "../Header_Files/defines.h"
#include "../Header_Files/classes.h"
#include "../Header_Files/masspoint.h"

class Masspoint
{
public:
	Masspoint(vec2 pos, float mass, float radius);
	Masspoint();

	void draw(sf::RenderWindow* window);
	void addForce(vec2 f);
	void physicsStep(float delta);
	void reset();
	void collision(PhysicsObject& p);

	vec2 position;
	vec2 velocity;
	vec2 force;
	float mass;
	float radius;

	static int nextID;
	int ID = 0;

	sf::CircleShape dot;

	
};