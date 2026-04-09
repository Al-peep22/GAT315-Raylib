#include "point_effector.h"
#include <raymath.h>

void PointEffector::Apply(std::vector<Body>& bodies)
{
    for (auto& body : bodies)
    {
        Vector2 dir = body.position - position;
        if (Vector2Length(dir) <= size)
        {
            Vector2 force = Vector2Normalize(dir) * forceMagnitude;
            body.AddForce(force);
        }
    }
}

void PointEffector::Draw()
{
    DrawCircleLinesV(position, size, WHITE);
}