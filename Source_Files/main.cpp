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
#define INITIAL_PARTICLES 0
#define INITIAL_TRIANGLES 2


bool thread_done[NUM_THREADS];

int stepcount = 0;

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


float wind = 0.01;

double fatal_error_threashold = 0.25;

vec2 v_vec = vec2(wind, 0);
vec2 g_vec = vec2(0, g);
vec2* v_vec_ptr = &v_vec;

std::list<PhysicsObject*> thread_objects[NUM_THREADS];

std::list<PhysicsObject*> p_chunks[CHUNK_X][CHUNK_Y];
std::list<PhysicsObject*> p_oldChunks[CHUNK_X][CHUNK_Y];
std::list<PhysicsObject*> p_out;
const int chunk_height = SCREEN_HEIGHT / CHUNK_Y;
const int chunk_width = SCREEN_WIDTH / CHUNK_X;

void drawScreen(sf::RenderWindow* window, std::list<PhysicsObject*>* particles);
void insert_into_chunk(PhysicsObject* p);
void reset_chunks();
void multithread_physics(int substeps, std::list<PhysicsObject*>* particles);
void physicsSubStepC(int xyid[]);
void physicsSubStepT(std::list<PhysicsObject*>* particles, int num);
void writeDebugData();
void createParticle(std::list<PhysicsObject*>* list, vec2 p, int r, sfCol c, bool f, double b, double m, vec2 init);
void createTriangle(std::list<PhysicsObject*>* list, vec2 p, vec2 pt[3], sfCol c, bool h, float b, float m, vec2 init);
void createParticle(std::list<PhysicsObject*>* list, vec2 p, int r, sfCol c, bool f, double b, double m);
void createTriangle(std::list<PhysicsObject*>* list, vec2 p, vec2 pt[3], sfCol c, bool h, float b, float m);
void calc_physics(std::list<PhysicsObject*>& objects);





int main()
{
	
	writeDebugData();
	//PhysicsObject::gravity = g;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	// Initialize Window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "2D Physics Engine in C++", sf::Style::Titlebar | sf::Style::Close, settings);

	if (!window.isOpen()) {
		std::cout << "Window not created" << std::endl;
		return -1;
	}
	window.setFramerateLimit(-1);
	window.setVerticalSyncEnabled(true);
	//window.setVerticalSyncEnabled(true);


	// Create Lists of Objects

	std::list<PhysicsObject*> objects;

	int noSpawned = 0;
	// Initially fill lists
	bool full = true;


	for (int i = 1; i <= INITIAL_PARTICLES; i++)
	{
		createParticle(&objects, vec2(i * 20, 200), 5, white, full, 0.9, i, vec2(10 * i * i, -100 + i));
		full = !full;

		max_physicsSteps = 0;
	}
	for (int i = 1; i <= INITIAL_TRIANGLES; i++)
	{
		createTriangle(&objects, vec2(100, i * 100 + 100), new vec2[3]{ vec2(-50, 0) ,vec2(50, 0) ,vec2(0, 100) }, sfCol(255,255,(i-1)*255), full, 0.7, 1, vec2(0, 0));
		full = !full;
		max_physicsSteps = 0;
	}


	// Variables for time calculation
	auto startTime = std::chrono::high_resolution_clock::now();


	// setup chuncks for faster calculation

	int x_spawn = 50;





	while (window.isOpen())
	{


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}



		multithread_physics(10, &objects);


		drawScreen(&window, &objects);
		

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
			for (PhysicsObject*& p : p_chunks[i][j])
			{
				//std::cout << i << " " << j << std::endl;
				if (p != nullptr)
				{
					for (int k = std::max(i - 1, 0); k <= std::min(i + 1, CHUNK_X - 1); k++)
					{
						for (int l = std::max(j - 1, 0); l <= std::min(j + 1, CHUNK_Y - 1); l++)
						{
							for (PhysicsObject*& q : p_chunks[k][l])
							{
								if (q != nullptr)
								{
									if (p->ID != q->ID)
									{
										p->collision(*q);
									}

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







void physicsSubStepT(std::list<PhysicsObject*>* particles, int num)
{

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








void drawScreen(sf::RenderWindow* window, std::list<PhysicsObject*>* particles)
{

	window->clear(bgCol);

	for (auto& p : *particles)
	{

		p->draw(window);
	}


	window->display();
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
}


void calc_physics(std::list<PhysicsObject*>& objects)
{
	for (PhysicsObject*& p : objects)
	{

		if (p != nullptr)
		{
			p->addForce(v_vec_ptr);
			p->physicsStep(0);
			p->reset();
		}

	}
}


void multithread_physics(int substeps, std::list<PhysicsObject*>* objects)
{
	std::chrono::steady_clock::time_point t0;
	std::chrono::steady_clock::time_point t1;
	std::chrono::steady_clock::time_point t00;
	std::chrono::steady_clock::time_point t01;
	t0 = Time::now();

	for (int i = 0; i < NUM_SUBSTEPS; i++)
	{
		stepcount++;


		int count = 0;
		for (PhysicsObject* p : *objects) {
			if (p != nullptr) {
				thread_objects[count % NUM_THREADS].push_back(p);
				count++;
			}
		}

		std::thread* threads = new std::thread[NUM_THREADS];

		for (int i = 0; i < NUM_THREADS; ++i) {
			if (!thread_objects[i].empty()) {
				threads[i] = std::thread(calc_physics, std::ref(thread_objects[i]));
			}
		}


		t00 = Time::now();
		for (int i = 0; i < NUM_THREADS; i++) {
			if (thread_objects[i].size() > 0) {
				t01 = Time::now();
				fsec fs = t01 - t00;
				double steptime = fs.count();
				if (steptime > 1)
				{
					std::cout << "waiting for thread to join " << i << std::endl;
				}
				threads[i].join();
			}
		}

		reset_chunks();
		for (auto& p : *objects)
		{
			insert_into_chunk(p);
		}


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
		threads[NUM_THREADS - 1] = std::thread(physicsSubStepT, objects, NUM_THREADS - 1);


		t00 = Time::now();
		for (int i = 0; i < NUM_THREADS; i++) {
			t01 = Time::now();
			fsec fs = t01 - t00;
			double steptime = fs.count();
			if (steptime > 1)
			{
				std::cout << "waiting for thread joining " << i << std::endl;
			}
			threads[i].join();

		}


		for (auto& p : *objects)
		{
			p->collisionUpdatePos();
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

void createParticle(std::list<PhysicsObject*>* list, vec2 p, int r, sfCol c, bool f, double b, double m, vec2 initialForce)
{
	PhysicsObject* par = new Particle(p, r, c, f, b, m);
	list->push_back(par);

	particle_count++;
}

void createTriangle(std::list<PhysicsObject*>* list, vec2 p, vec2 pt[3], sfCol c, bool h, float b, float m, vec2 initialForce)
{
	PhysicsObject* tri = new Triangle(p, pt, c, h, b, m);
	list->push_back(tri);
	std::cout << tri->ID << std::endl;

	triangle_count++;
}

void createParticle(std::list<PhysicsObject*>* list, vec2 p, int r, sfCol c, bool f, double b, double m)
{
	PhysicsObject* par = new Particle(p, r, c, f, b, m);
	list->push_back(par);
	particle_count++;
}

void createTriangle(std::list<PhysicsObject*>* list, vec2 p, vec2 pt[3], sfCol c, bool h, float b, float m)
{
	PhysicsObject* tri = new Triangle(p, pt, c, h, b, m);
	list->push_back(tri);
	triangle_count++;
}

void insert_into_chunk(PhysicsObject* p)
{

	switch (p->type)
	{
	case PH_PAR:
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
	break;
	case PH_TRI:
		int x1 = std::min(p->points[0].x, std::min(p->points[1].x, p->points[2].x));
		int x2 = std::max(p->points[0].x, std::max(p->points[1].x, p->points[2].x));

		int y1 = std::min(p->points[0].y, std::min(p->points[1].y, p->points[2].y));
		int y2 = std::max(p->points[0].y, std::max(p->points[1].y, p->points[2].y));
		bool out = false;

		for (int i = x1 / chunk_width; i <= x2 / chunk_width && i < CHUNK_X && i >= 0; i++) {

			for (int j = y1 / chunk_height; j <= y2 / chunk_height; j++) {

				if (j >= CHUNK_Y || j < 0)
				{
					if (!out) {
						p_out.push_back(p);
						out = true;
					}
				}
				else
				{
					p_chunks[i][j].push_back(p);
				}

			}
		}


		break;
	}

}

