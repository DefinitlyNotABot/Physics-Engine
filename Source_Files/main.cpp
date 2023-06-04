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

int main()
{
	//PhysicsObject::gravity = g;

	// Initialize Window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Physics Engine", sf::Style::Titlebar | sf::Style::Close);

	if (!window.isOpen()) {
		std::cout << "Window not created" << std::endl;
		return -1;
	}
	window.setFramerateLimit(-1);
	//window.setVerticalSyncEnabled(true);


	// Create Lists of Objects

	std::list<Particle> particles;


	// Initially fill lists


	Particle p1 = Particle(vec2(100, 200), 10, white, false, 1, 1);
	particles.push_back(p1);
	Particle p2 = Particle(vec2(400, 200), 10, white, true, 1, 1);
	particles.push_back(p2);





	// Variables for time calculation
	auto startTime = std::chrono::high_resolution_clock::now();
	float deltaTime = 0.0f;
	float deltaCount = 0.0f;

	float avgDelta = 0.0f;

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
			// p.addForce(g_vec);
			p.addForce(v_vec);
			p.addForce(vec2(0, -0.1f));
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
		if (deltaCount >= 0) {
			window.clear(bgCol);

			for (auto& p : particles)
			{
				p.draw(window);
			}
			// td::cout << 1 / deltaTime << std::endl;
			deltaCount = 0;
		}

		window.display();

		avgDelta = (deltaTime + avgDelta) / 2;

		startTime = currentTime;


	}

	std::cout << 1 / avgDelta << std::endl;

	return 0;
}



void physicsSubStep(double delta, int substeps)
{
	
}