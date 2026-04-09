#include "GravitationalEffector.h"
#include <cmath>

void GravitationalEffector::Apply(std::vector<Body>& bodies)
{
    for (size_t i = 0; i < bodies.size(); i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            Body& A = bodies[i];
            Body& B = bodies[j];

            // Skip static bodies
            if (A.bodyType != BodyType::Dynamic && B.bodyType != BodyType::Dynamic)
                continue;

            // STEP 1: Direction from B ? A
            Vector2 dir = Vector2Subtract(A.position, B.position);

            // STEP 2: Distance
            float distance = Vector2Length(dir);

            // STEP 3: Clamp distance
            distance = fmaxf(distance, 1.0f);

            // STEP 4: Force magnitude
            float forceMag = strength * (A.mass * B.mass) / (distance * distance);

            // STEP 5: Force Vector (Normalized direction * magnitude)
            Vector2 force = Vector2Scale(Vector2Normalize(dir), forceMag);

            // STEP 6: Apply equal and opposite forces
            A.AddForce(Vector2Negate(force));
            B.AddForce(force);
        }
    }
}