#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>
#include <iostream>
#include <list>
#include <chrono>
#include <windows.h>
#include <thread>


// define constants

# define PI	3.14159265358979323846  // Pi
# define g	0.5f	// Gravity
# define AIR_RESSISTANCE 0.95f	// Air resistance, multiplied on movement vec

# define SCREEN_WIDTH 800
# define SCREEN_HEIGHT 600
# define ANG_MOMENTUM_MULTIPLY 0.00001
# define FLOOR_MASS std::numeric_limits<float>::max()



// Define custom types

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef sf::Vector2f sfVec2;
typedef sf::Color sfCol;

typedef glm::vec2 vec2;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

// define enums

enum ang_between_vec_types
{
	ANG_ABSOLUTE,
	ANG_CLOCK_UNSIGNED,
	ANG_CLOCK_SIGNED,
	ANG_CCLOCK_UNSIGNED,
	ANG_CCLOCK_SIGNED
};

enum physics_object_types
{
	PH_OBJ,
	PH_PAR,
	PH_TRI
};

// define classes

class PhysicsObject;
class Physics;

// define functions

sf::Vector2f vec2_2_sfVec2(glm::vec2* x);
vec2 rotate_vector(vec2 v, float ang);
float ang_between_vec(vec2 a, vec2 b, int type);
float cross_2D(vec2 a, vec2 b);
bool PointInTriangle(vec2 pt, vec2 v1, vec2 v2, vec2 v3);
vec2 calculateIntersection(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p4);