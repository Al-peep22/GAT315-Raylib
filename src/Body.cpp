#include "Body.h"
#include "raymath.h"

void Body::AddForce(Vector2 force)
{
    acceleration += force / mass;
}

void Body::Step(float dt)
{
    // Semi-implicit Euler (your code uses this)
    velocity += acceleration * dt;
    position += velocity * dt;
}