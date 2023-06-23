#pragma once
#include "../Header_Files/defines.h"
#include "PhysicsObject.h"


class Triangle : public PhysicsObject
{
public:
	Triangle(vec2 pos, vec2 pts[3], sfCol col, bool hollow, float bouncyness, float m);
	void draw(sf::RenderWindow* window) override;
	void physicsStep() override;

	void collision(const Triangle& p);
	void collisionUpdatePos();
	void updateTriangle();


	float speed = 0.0f;
	vec2 position;

	vec2 positionSave;

	float bouncyness;

	float mass;

	sfCol color;
	bool hollow;

	vec2 relativePoints[3];
	vec2 points[3];

	sf::VertexArray triangle;

private:
	void createTriangle();

	

};