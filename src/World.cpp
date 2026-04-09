#include "World.h"
#include "raylib.h"
#include "raymath.h"
#include <Integrater.h>

void World::AddBody(const Body& body)
{
    bodies.push_back(body);
}

void World::Step(float dt)
{
    // Reset acceleration
    for (auto& body : bodies)
        body.acceleration = { 0, 0 };

    // Apply world gravity
    for (auto& body : bodies)
        body.AddForce(gravity * body.gravityScale * 100.0f, ForceMode::Acceleration);

    // Apply effectors (NEW)
    for (auto& effector : effectors)
        effector->Apply(bodies);

    // Right-click radial force
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 pos = GetMousePosition();
        for (auto& body : bodies)
        {
            Vector2 dir = Vector2Subtract(body.position, pos);
            if (Vector2Length(dir) <= 100.0f)
            {
                Vector2 force = Vector2Scale(Vector2Normalize(dir), 10000.0f);
                body.AddForce(force);
            }
        }
    }

    // Integrate
    for (auto& body : bodies)
        SemiImplicitEuler(body, dt);

    // Collisions
    for (auto& body : bodies)
    {
        if (body.position.x + body.size > GetScreenWidth())
        {
            body.position.x = GetScreenWidth() - body.size;
            body.velocity.x *= -body.restitution;
        }
        if (body.position.x - body.size < 0)
        {
            body.position.x = body.size;
            body.velocity.x *= -body.restitution;
        }
        if (body.position.y + body.size > GetScreenHeight())
        {
            body.position.y = GetScreenHeight() - body.size;
            body.velocity.y *= -body.restitution;
        }
    }
}

void World::Draw(Texture2D wabbit) const
{
    for (const auto& body : bodies)
        DrawTexture(wabbit, (int)body.position.x, (int)body.position.y, WHITE);
}

void World::Draw()
{
	for (const auto& body : bodies) body.Draw();
}
