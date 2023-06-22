#include "../Header_Files/defines.h"
#include "../Header_Files/classes.h"

#include <thread>
#include <iostream>
#include <chrono>
#include <list>

#define CHUNK_X 20
#define CHUNK_Y 20
#define NUM_THREADS 6
#define NUM_SUBSTEPS 2
#define OUTPUT_DEBUG false


bool thread_done[NUM_THREADS];

sfCol bgCol = sfCol(0, 0, 0);
sfCol white = sfCol(255, 255, 255);

int min_physicsSteps = -1;
int max_physicsSteps = 0;
int min_c = 0;
int max_c = 0;
int c_p_steps = 0;

int spawn_x = 10;
int previous_fatal_error = 0;

int particle_count = 0;
int triangle_count = 0;


float wind = 0.2;

double fatal_error_threashold = 0.25;

vec2 v_vec = vec2(wind, 0);
vec2 g_vec = vec2(0, g);
vec2* v_vec_ptr = &v_vec;



std::list<Particle*> p_chunks[CHUNK_X][CHUNK_Y];
std::list<Particle*> p_oldChunks[CHUNK_X][CHUNK_Y];
std::list<Particle*> p_out;
const int chunk_height = SCREEN_HEIGHT / CHUNK_Y;
const int chunk_width = SCREEN_WIDTH / CHUNK_X;

void drawScreen(sf::RenderWindow* window, std::list<Particle>* particles, std::list<Triangle>* triangles);
void insert_into_chunk(Particle* p);
void reset_chunks();
void move_to_chunk(Particle* p, int ox, int oy);
void multithread_physics(int substeps, std::list<Particle>* particles, std::list<Triangle>* triangles);
void physicsSubStepC(int xyid[]);
void physicsSubStepT(std::list<Particle>* particles, int num);
void writeDebugData();
void createParticle(std::list<Particle>* list, vec2 p, int r, sfCol c, bool f, double b, double m);
void createTriangle(std::list<Triangle>* list, vec2 p, vec2 pt[3], sfCol c, bool h, float b, float m);





int main()
{
	writeDebugData();
	//PhysicsObject::gravity = g;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	// Initialize Window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Physics Engine", sf::Style::Titlebar | sf::Style::Close, settings);

	if (!window.isOpen()) {
		std::cout << "Window not created" << std::endl;
		return -1;
	}
	window.setFramerateLimit(-1);
	window.setVerticalSyncEnabled(true);
	//window.setVerticalSyncEnabled(true);


	// Create Lists of Objects

	std::list<Particle> particles;
	std::list<Triangle> triangles;

	int noSpawned = 0;
	// Initially fill lists
	bool full = true;


	for (int i = 1; i <= 5; i++)
	{
		//createParticle(&particles, vec2(i * 10, 200), 2, white, full, 0.5, 1);
		full = !full;
		
		max_physicsSteps = 0;
	}
	for (int i = 1; i <= 1; i++)
	{
		createTriangle(&triangles, vec2(i*100+100, 100), new vec2[3]{ vec2(-50, 0) ,vec2(50, 0) ,vec2(0, 75) }, white, true, 0.9, 1);

		max_physicsSteps = 0;
	}


	// Variables for time calculation
	auto startTime = std::chrono::high_resolution_clock::now();


	// setup chuncks for faster calculation

	int x_spawn = 50;



	

	while (window.isOpen())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = currentTime - startTime;






		// Convert the duration to seconds
		startTime = std::chrono::high_resolution_clock::now();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || max_physicsSteps > 100)
		{
			//createParticle(&particles, vec2(spawn_x, 100), 2, white, true, 0.5, max_physicsSteps % 6 + 1);
			spawn_x += 11;
			spawn_x = spawn_x % SCREEN_WIDTH;
			writeDebugData();
		}


		multithread_physics(10, &particles, &triangles);

		

		

		drawScreen(&window, &particles, &triangles);

	}

	return 0;
}


void physicsSubStepC(int xyid[])
{
	int x0 = xyid[0];
	int x1 = xyid[1];
	int y0 = xyid[2];
	int y1 = xyid[3];
	int id = xyid[4];
	for (int i = x0; i <= x1; i++)
	{
		for (int j = y0; j <= y1; j++)
		{
			p_oldChunks[i][j] = p_chunks[i][j];
		}
	}

	for (int i = x0; i <= x1; i++)
	{
		for (int j = y0; j <= y1; j++)
		{
			for (Particle*& p : p_oldChunks[i][j])
			{

				if (p != nullptr)
				{
					p->reset();
					p->addForce(v_vec_ptr);
					p->physicsStep();
				}

			}
		}
	}
	std::chrono::steady_clock::time_point t00;
	std::chrono::steady_clock::time_point t01;
	t00 = Time::now();

	thread_done[id] = true;
	for (int i = 0; i < NUM_THREADS; i++) {

		while (!thread_done[i]) {
			t01 = Time::now();
			fsec fs = t01 - t00;
			double steptime = fs.count();
			//std::cout << "waiting for thread " << steptime << std::endl;
			if (steptime > fatal_error_threashold)
			{
				std::cout << "Thread " << id << " waiting for thread " << i << std::endl;
				i += 1;
				previous_fatal_error++;
				writeDebugData();
			}
		
		}
	}

	while (thread_done[0]) { ; }

	for (int i = x0; i <= x1; i++)
	{
		for (int j = y0; j <= y1; j++)
		{
			for (auto& p : p_chunks[i][j])
			{
				if (p != nullptr)
				{
					for (int k = std::max(i - 1, 0); k <= std::min(i + 1, CHUNK_X - 1); k++)
					{
						for (int l = std::max(j - 1, 0); l <= std::min(j + 1, CHUNK_Y - 1); l++)
						{

							for (auto& q : p_chunks[k][l])
							{
								if (q != nullptr && p->ID != q->ID)
								{
									p->collision(*q);
								}
							}

						}
					}

					if (j == 0)
					{
						for (auto& q : p_out)
						{
							if (q != nullptr && p->ID != q->ID)
							{
								p->collision(*q);
							}
						}
					}
				}
			}
		}
	}


}







void physicsSubStepT(std::list<Particle>* particles, int num)
{
	std::list<Particle*> p_oldOut = p_out;

	for (auto& p : p_oldOut)
	{
		if (p != nullptr)
		{
			p->reset();
			p->addForce(v_vec_ptr);
			p->physicsStep();
			move_to_chunk(p, -1, -1);
		}
	}
	thread_done[num] = true;
	std::chrono::steady_clock::time_point t00;
	std::chrono::steady_clock::time_point t01;
	t00 = Time::now();
	for (int i = 0; i < NUM_THREADS; i++) {
		while (!thread_done[i]) 
		{
			t01 = Time::now();
			fsec fs = t01 - t00;
			double steptime = fs.count();
			//std::cout << "waiting for thread " << steptime << std::endl;
			if (steptime > fatal_error_threashold)
			{
				std::cout << "Thread out waiting for thread " << i << std::endl;
				i += 1;
				previous_fatal_error++;
				writeDebugData();
			}
		
		}
	}
	while (thread_done[0]) { ; }
	for (auto& p : p_out)
	{
		if (p != nullptr)
		{
			for (int k = 0; k < CHUNK_X; k++)
			{

				for (auto& q : p_chunks[k][0])
				{
					if (q != nullptr)
					{
						if (p->ID != q->ID) {
							p->collision(*q);
						}
					}
				}
			}
		}
	}
}








void drawScreen(sf::RenderWindow* window, std::list<Particle>* particles, std::list<Triangle>* triangles)
{

	window->clear(bgCol);

	for (auto& p : *particles)
	{
		p.draw(window);
	}
	for (auto& p : *triangles)
	{
		p.draw(window);
	}

	window->display();
}

void insert_into_chunk(Particle* p)
{
	int x = p->position.x;
	int y = p->position.y;

	if (y < 0)
	{
		p_out.push_back(p);
		return;
	}
	p_chunks[x / chunk_width][y / chunk_height].push_back(p);
}

void move_to_chunk(Particle* p, int ox, int oy)
{
	if (ox == -1 || oy == -1) {
		p_out.remove(p);
		insert_into_chunk(p);
		return;
	}

	p_chunks[ox][oy].remove(p);
	insert_into_chunk(p);
}

void reset_chunks()
{
	for (int i = 0; i < CHUNK_X; i++)
	{
		for (int j = 0; j < CHUNK_X; j++)
		{
			p_chunks[i][j].clear();
		}
	}
	p_out.clear();
	p_out.push_back(nullptr);
}

void multithread_physics(int substeps, std::list<Particle>* particles, std::list<Triangle>* triangles)
{
	std::chrono::steady_clock::time_point t0;
	std::chrono::steady_clock::time_point t1;
	t0 = Time::now();

	for (int i = 0; i < NUM_SUBSTEPS; i++)
	{

		for (auto& p : *triangles)
		{
			p.reset();
			p.addForce(v_vec_ptr);
			p.physicsStep();

		}






		for (int i = 0; i < NUM_THREADS; i++) {
			thread_done[i] = false;
		}
		

		reset_chunks();

		for (auto& p : *particles)
		{
			insert_into_chunk(&p);
		}



		std::thread* threads = new std::thread[NUM_THREADS];

		int xot = CHUNK_X / (NUM_THREADS - 1);

		// Create and launch threads
		for (int i = 0; i < NUM_THREADS - 1; ++i) {
			//std::cout << "Waiting for thread " << i << " to be started" << std::endl;
			int x0 = i * xot;
			int x1 = (i + 1) * xot - 1;
			int y0 = 0;
			int y1 = CHUNK_Y - 1;
			threads[i] = std::thread(physicsSubStepC, new int[5] {x0, x1, y0, y1, i});

		}
		threads[NUM_THREADS - 1] = std::thread(physicsSubStepT, particles, NUM_THREADS - 1);

		std::chrono::steady_clock::time_point t00;
		std::chrono::steady_clock::time_point t01;
		t00 = Time::now();
		for (int i = 0; i < NUM_THREADS; i++) {
			//std::cout << "Waiting for thread " << i << " to finnish" << std::endl;
			while (!thread_done[i]) 
			{ 
				t01 = Time::now();
				fsec fs = t01 - t00;
				double steptime = fs.count();
				//std::cout << "waiting for thread " << steptime << std::endl;
				if (steptime > 1)
				{
					std::cout << "waiting for thread " << i << std::endl;
				}
			}
		}


		reset_chunks();
		for (int i = 0; i < CHUNK_X; i++)
		{
			for (int j = 0; j < CHUNK_Y; j++)
			{

				p_chunks[i][j] = p_oldChunks[i][j];

			}
		}


		thread_done[0] = false;



		t00 = Time::now();
		// Wait for threads to finish
		for (int i = 0; i < NUM_THREADS; i++) {
			//std::cout << "Waiting for thread " << i << " to join" << std::endl;
			t01 = Time::now();
			fsec fs = t01 - t00;
			double steptime = fs.count();
			//std::cout << "waiting for thread " << steptime << std::endl;
			if (steptime > 1)
			{
				std::cout << "waiting for thread to join " << i << std::endl;
			}
			threads[i].join();
		}




		for (auto& p : *particles)
		{
			p.collisionUpdatePos();
		}
		delete[] threads;
	}

	t1 = Time::now();
	fsec fs = t1 - t0;
	int steptime = (int)(1 / fs.count());
	min_c++;
	max_c++;


	if (steptime < min_physicsSteps || min_physicsSteps == -1)
	{
		min_physicsSteps = steptime;
		writeDebugData();
		min_c = 0;
	}
	else if (steptime > max_physicsSteps)
	{
		max_physicsSteps = steptime;
		writeDebugData();
		max_c = 0;
	}

	if (min_c > 1000) {
		min_physicsSteps = steptime;
		writeDebugData();
		min_c = 0;
	}
	if (max_c > 1000)
	{
		max_physicsSteps = steptime;
		writeDebugData();
		max_c = 0;
	}

}

void writeDebugData()
{
	if (OUTPUT_DEBUG) 
	{
		system("cls");
		std::cout << "Number of Chunks: " << CHUNK_X * CHUNK_Y << std::endl;
		std::cout << "chunk dims: " << chunk_width << "x" << chunk_height << std::endl;
		std::cout << "Running on " << NUM_THREADS << " Threads " << std::endl;
		std::cout << "Chunks / Thread: " << (CHUNK_X * CHUNK_Y) / (NUM_THREADS - 1) << std::endl;
		std::cout << "\nmin: " << min_physicsSteps << std::endl;
		std::cout << "max: " << max_physicsSteps << std::endl;
		std::cout << "Particles: " << particle_count << std::endl;
		std::cout << "Fatal errors avoided: " << previous_fatal_error << std::endl;
	}
	
}

void createParticle(std::list<Particle>* list, vec2 p, int r, sfCol c, bool f, double b, double m)
{
	Particle par = Particle(p, r, c, f, b, m);
	list->push_back(par);
	particle_count++;
}

void createTriangle(std::list<Triangle>* list, vec2 p, vec2 pt[3], sfCol c, bool h, float b, float m)
{
	std::cout << "created triangle" << std::endl;
	Triangle par = Triangle(p, pt, c, h, b, m);
	list->push_back(par);
	triangle_count++;
}