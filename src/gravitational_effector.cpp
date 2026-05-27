#include "gravitational_effector.h"
#include <cmath>

void GravitationalEffector::Apply(std::vector<Body>& ibodies)
{
    std::vector<Body*> bodies;
    CollectBodiesInside(ibodies, bodies);

    for (size_t i = 0; i < bodies.size(); i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            Body& A = *bodies[i];
            Body& B = *bodies[j];

            if (A.bodyType != BodyType::Dynamic && B.bodyType != BodyType::Dynamic)
                continue;

            Vector2 dir = Vector2Subtract(B.position, A.position);
            float distance = fmaxf(Vector2Length(dir), 1.0f);

            float forceMag = strength * (A.mass * B.mass) / (distance * distance);
            Vector2 force = Vector2Scale(Vector2Normalize(dir), forceMag);

            A.AddForce(force);
            B.AddForce(Vector2Negate(force));
        }
    }
}



void GravitationalEffector::Draw() {
    Effector::Draw();
    DrawCircleV(position,size, Fade(BLUE, 0.2f));
}