#pragma once
#include "Body.h"
#include <vector>

enum class EffectorType
{
	Gravitation,
	Point,
	Area,
	Drag
};

//creates a circle that applies a force to bodies within its radius. The force can be either attractive or repulsive based on the sign of the forceMagnitude.
class Effector {
public: 
	Effector(Vector2 position, float size) : position(position), size(size) {}
	virtual void Apply(std::vector<Body>& bodies) = 0;
	virtual void Draw();

	void CollectBodiesInside(std::vector<Body>& ibodies, std::vector<Body*>& obodies);
protected:
	Vector2 position;
	float size = 100.0f;
};