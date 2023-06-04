#pragma once
#include "../Header_Files/defines.h"

class Polygon
{
public:
	Polygon(vec2 points[]);

	void rotate(float deg);
	void move(float x, float y);

	sf::ConvexShape getTriangle();

private:

	sf::ConvexShape triangle;

	float rotation;
	float angMomentum;

};