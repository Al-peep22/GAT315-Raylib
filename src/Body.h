#pragma once
#include "raylib.h"

struct Body
{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    float size;
    float restitution;

    void AddForce(Vector2 force);
    void Step(float dt); // will use SemiImplicitEuler
};