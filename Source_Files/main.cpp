#include "../Header_Files/defines.h"
#include "../Header_Files/classes.h"

#define CHUNK_X 50
#define CHUNK_Y 50



sfCol bgCol = sfCol(0, 0, 0);
sfCol white = sfCol(255, 255, 255);


vec2 g_vec = vec2(0, g);

float wind = 0.01;
vec2 v_vec = vec2(wind, 0);

vec2* v_vec_ptr = &v_vec;



std::list<Particle*> p_chunks[CHUNK_X][CHUNK_Y];
std::list<Particle*> p_oldChunks[CHUNK_X][CHUNK_Y];
std::list<Particle*> p_out;
const int chunk_height = SCREEN_HEIGHT / CHUNK_Y;
const int chunk_width = SCREEN_WIDTH / CHUNK_X;
//funcs

void physicsSubStep(std::list<Particle>* particles, int x0, int x1, int y0, int y1);
void physicsSubStep(std::list<Particle>* particles, bool out_of_map);
void drawScreen(sf::RenderWindow* window, std::list<Particle>* particles);
void insert_into_chunk(Particle* p);
void reset_chunks();
void move_to_chunk(Particle* p, int ox, int oy);
void multithread_physics(int substeps, std::list<Particle>* particles);


int main()
{

	std::cout << "chunk dims: " << chunk_width << "x" << chunk_height << std::endl;
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
	for (int i = 1; i <= 50; i++)
	{
		Particle p = Particle(vec2(i * 10, 200), 2, white, full, 1, 1);
		particles.push_back(p);
		full = !full;
		particleCount++;
	}

	Particle control = Particle(vec2(5, 200), 1, white, full, 1, 1);

	// Variables for time calculation
	auto startTime = std::chrono::high_resolution_clock::now();
	uint64 deltaTime = 0.0f;

	float avgDelta = 0.0f;


	// setup chuncks for faster calculation

	int x_spawn = 50;

	uint64* deltaTime_ptr = &deltaTime;


	vec2 initVec = vec2(10, 0);

	while (window.isOpen())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = currentTime - startTime;






		// Convert the duration to seconds
		deltaTime = duration.count();
		startTime = std::chrono::high_resolution_clock::now();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		multithread_physics(5, &particles);

		drawScreen(&window, &particles);




		avgDelta = (deltaTime + avgDelta) / 2;
	}

	std::cout << avgDelta << std::endl;

	return 0;
}



void physicsSubStep(int x0, int x1, int y0, int y1)
{

	for (int i = x0; i <= x1; i++)
	{
		for (int j = y0; j <= y1; j++)
		{
			p_oldChunks[i][j] = p_chunks[i][j];
		}
	}

	reset_chunks();
	for (int i = x0; i <= x1; i++)
	{
		for (int j = y0; j <= y1; j++)
		{
			for (Particle*& p : p_oldChunks[i][j])
			{
				
				if (p != nullptr)
				{
					std::cout << "heyho" << std::endl;
					p->reset();
					p->addForce(v_vec_ptr);
					p->physicsStep();
					move_to_chunk(p, i, j);
				}
				
			}
		}
	}

	for (int i = x0; i <= x1; i++)
	{
		for (int j = y0; j <= y1; j++)
		{
			for (auto& p : p_chunks[i][j])
			{
				if (p != nullptr)
				{
					for (int k = -1; k <= 1; k++)
					{
						for (int l = -1; l <= 1; l++)
						{
							if (i + k > 0 && i + k < CHUNK_X && j + l > 0 && j + l < CHUNK_Y)
							{
								for (auto& q : p_chunks[i + k][j + l])
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
					if (j == 0)
					{
						for (auto& q : p_out)
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
	}

}

void physicsSubStep(std::list<Particle>* particles, bool over_screen)
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

	for (auto& p : p_oldOut)
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

void drawScreen(sf::RenderWindow* window, std::list<Particle>* particles)
{

	(*window).clear(bgCol);

	for (auto& p : *particles)
	{
		p.draw(*window);
	}


	(*window).display();
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
			p_chunks[i][j].push_back(nullptr);
		}
	}
	p_out.clear();
	p_out.push_back(nullptr);
}

void multithread_physics(int substeps, std::list<Particle>* particles)
{
	reset_chunks();

	for (auto& p : *particles)
	{
		insert_into_chunk(&p);
	}

	for (int i = 0; i < substeps; i++)
	{
		physicsSubStep(0, 49, 0, 49);
		physicsSubStep(particles, true);
		for (auto& p : *particles)
		{
			p.collisionUpdatePos();
		}
	}




	
}