#pragma once
#include "../Header_Files/defines.h"

class PhysicsObject
{
public:
	PhysicsObject();

	void addForce(vec2 dir);
	void reset();

	virtual void draw(sf::RenderWindow& window);
	virtual void physicsStep(double delta);


	float rotation;
	float angMomentum;
	float energy_k;
	float energy_p;

	vec2 force;
	vec2 moveDir;

	vec2 moveDirSave;

	static int nextID;
	int ID;

	float delta;

	static float gravity;
};