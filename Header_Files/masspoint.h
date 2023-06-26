#pragma once
#include "../Header_Files/defines.h"

class Masspoint
{
public:
	Masspoint(vec2 pos, float mass, float radius);
	Masspoint();

	void draw(sf::RenderWindow* window);

	vec2 position;
	vec2 velocity;
	vec2 force;
	float mass;
	float radius;

	sf::CircleShape dot;

	
};