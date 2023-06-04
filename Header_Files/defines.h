#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>

// define constants

# define PI	3.14159265358979323846  // Pi
# define g	0.1f	// Gravity
# define airResistance 0.95f	// Air resistance, multiplied on movement vec



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


// define classes

class PhysicsObject;