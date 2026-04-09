#pragma once
#include <vector>
#include "Body.h"
#include "Effector.h"

struct World
{
public:
    World() {
        gravity = { 0, 9.81f };
        bodies.reserve(1000);
    }

    void AddBody(const Body& body);
    void AddEffector(Effector* effector)
    {
        effectors.push_back(effector);
    }
    void Step(float dt);
    void Draw(Texture2D wabbit) const;
    void Draw();

private:
    std::vector<Body> bodies;
    Vector2 gravity;

    std::vector<Effector*> effectors;
};