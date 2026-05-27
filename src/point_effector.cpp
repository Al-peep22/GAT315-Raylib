#include "point_effector.h"
#include <raymath.h>

void PointEffector::Apply(std::vector<Body>& ibodies)
{
    std::vector<Body*> bodies;
    CollectBodiesInside(ibodies, bodies);

    for (auto& body : bodies)
    {
        Vector2 dir = Vector2Normalize(Vector2Subtract(body->position, position));
        Vector2 force = Vector2Scale(dir, forceMagnitude);
        body->AddForce(force);
    }
}



void PointEffector::Draw()
{
    Effector::Draw();
    DrawCircleV(position, size, Fade(RED,0.2f));
}