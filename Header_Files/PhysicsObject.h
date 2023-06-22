#pragma once
#include "../Header_Files/defines.h"

class PhysicsObject
{
public:
	PhysicsObject();

	void addForce(vec2* dir);
	void reset();

	virtual void draw(sf::RenderWindow* window);
	virtual void physicsStep();


	float rotation = 0;
	float angMomentum = 0;
	float energy_k = 0;
	float energy_p = 0;
	float energy_all = 0;
	std::chrono::steady_clock::time_point t0;
	std::chrono::steady_clock::time_point t1;

	vec2 force = vec2(0,0);
	vec2 moveDir = vec2(0, 0);

	vec2 moveDirSave = vec2(0, 0);

	static int nextID;
	int ID = 0;

	float delta = 0;

	static float gravity;
	bool step_calculated = false;
};