#include <iostream>
#include <list>
#include <chrono>


#include "../Header_Files/defines.h"
#include "../Header_Files/classes.h"



sfCol bgCol = sfCol(0, 0, 0);
sfCol white = sfCol(255, 255, 255);


vec2 g_vec = vec2(0, g);

float wind = 0.01;
vec2 v_vec = vec2(wind, 0);

const int chunk_x = 4;
const int chunk_y = 4;


int main()
{
	//PhysicsObject::gravity = g;

	// Initialize Window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Physics Engine", sf::Style::Titlebar | sf::Style::Close);

	if (!window.isOpen()) {
		std::cout << "Window not created" << std::endl;
		return -1;
	}
	window.setFramerateLimit(-1);
	//window.setVerticalSyncEnabled(true);


	// Create Lists of Objects

	std::list<Particle> particles;

	int particleCount = 0;
	int noSpawned = 0;
	// Initially fill lists
	bool full = true;
	for (int i = 1; i <= 5; i++)
	{
		Particle p = Particle(vec2(i*100, 200), 1, white, full, 1, 1);
		particles.push_back(p);
		full = !full;
		particleCount++;
	}

	int delaytime = 1000;

	// Variables for time calculation
	auto startTime = std::chrono::high_resolution_clock::now();
	float deltaTime = 0.0f;
	float deltaCount = 0.0f;

	float avgDelta = 0.0f;


	// setup chuncks for faster calculation


	while (window.isOpen())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = currentTime - startTime;
		deltaTime = duration.count();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (auto& p : particles)
		{
			p.reset();
			p.addForce(v_vec);
			p.physicsStep(deltaTime);
		}
		for (auto& p : particles)
		{
			for (auto& q : particles)
			{
				if (p.ID != q.ID) {
					p.collision(q);
				}

			}
		}
		for (auto& p : particles)
		{
			p.collisionUpdatePos();
		}


		deltaCount += deltaTime;

		window.clear(bgCol);

		for (auto& p : particles)
		{
			p.draw(window);
		}


		if (noSpawned >= delaytime && 1 / deltaTime > 60)
		{
			Particle p = Particle(vec2(100, 200), 1, white, full, 1, 1);
			particles.push_back(p);
			full = !full;
			particleCount++;
			std::cout << particleCount << std::endl;
			std::cout << 1 / deltaTime << std::endl;
			std::cout << std::endl;
			deltaCount = 0;
			noSpawned = 0;
			delaytime -= 20;
		}


		window.display();

		avgDelta = (deltaTime + avgDelta) / 2;

		startTime = currentTime;

		noSpawned++;
	}

	std::cout << 1 / avgDelta << std::endl;

	return 0;
}



void physicsSubStep(double delta, int substeps)
{
	
}