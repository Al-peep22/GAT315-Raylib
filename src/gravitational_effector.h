#pragma once
#include "Effector.h"
#include "Body.h"
#include "raymath.h"

class GravitationalEffector : public Effector
{
public:
    GravitationalEffector(Vector2 position, float size, float strength) :
		Effector(position, size),
        strength(strength) 
    { }

    void Apply(std::vector<Body>& bodies) override;

    void Draw();

private:
    float strength;
};