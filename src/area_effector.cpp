#include "area_effector.h"

void AreaEffector::Apply(std::vector<Body>& ibodies)
{
    std::vector<Body*> bodies;
    CollectBodiesInside(ibodies, bodies);

    Vector2 dir = Vector2Normalize({ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) });

    for (auto& body : bodies)
    {
        Vector2 force = Vector2Scale(dir, forceMagnitude);
        body->AddForce(force);
    }
}


void AreaEffector::Draw()
{
    Effector::Draw();
    DrawCircleV(position, size, Fade(GREEN, 0.2f));
}