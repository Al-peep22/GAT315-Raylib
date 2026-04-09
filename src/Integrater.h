#pragma once
#include "Body.h"
#include "raymath.h"

inline void ExpliciteEuler(Body& body, float dt)
{
	body.position += body.velocity * dt;
	body.velocity += body.acceleration * dt;
	body.velocity *= (1.0f / (1.0f + dt * body.damping * dt));
}

inline void SemiImplicitEuler(Body& body, float dt)
{
	body.velocity += body.acceleration * dt;
	body.velocity *= (1.0f / (1.0f + dt * body.damping * dt));
	body.position += body.velocity * dt;
}