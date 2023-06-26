#pragma once
#include "../Header_Files/defines.h"

class PhysicsObject
{
public:
	PhysicsObject();

	void addForce(vec2* dir);
	void reset();
	void updateEnergy();

	virtual void draw(sf::RenderWindow* window);
	virtual void physicsStep(int chunk_id);
	virtual void collision(PhysicsObject& p);
	virtual void collisionUpdatePos();


	// general

	float rotation = 0;
	float rotationSpeed = 0;
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

	int type;
	float delta = 0;

	static float gravity;
	bool step_calculated = false;

	bool on_ground = false;
	

	float speed = 0.0f;
	vec2 position;
	vec2 center;

	vec2 positionSave;

	float bouncyness;

	float mass;

	
	sfCol color;
	bool hollow;

	std::list<int> interacted_objects;

	// particle

	float radius;
	sf::CircleShape dot;



	// Triangle

	vec2 relativePoints[3];
	vec2 points[3];

	sf::VertexArray triangle;

	// Softbody

	float stiffness;
	float masses;
	float dampness;
	int resolution;
	int width;
	int height;

	
};